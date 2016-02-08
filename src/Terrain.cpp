#include "Terrain.h"

Terrain::Terrain(GLint shaderProgram, unsigned int xDim, unsigned int zDim, Camera *const camera, Noise *const noise)
    : mXDim(xDim)
    , mZDim(zDim)
    , mTotalVertices(xDim * zDim)
    , mHeightMapTexture()
    , mNormalMapTexture("../src/textures/rocknormalmap.jpg")
    , mWhiteNoiseTexture("../src/textures/whitenoise.png")
    , mLightPos(10.0f, 50.0f, 0.0f)
{
    this->mShaderProgram = shaderProgram;
    this->mCamera = camera;
    this->mNoise = noise;

    mUseNormals = false;
    mUseNormalMap = false;

    mTerrainPosRange = 50.0f;
    mTerrainMinPos = -25.0f;
    mHeightMapTerrainRatio = 1;
    mElementsPerVertex = 1;
    mFloatsPerVertex = 3;
    mDrawGrid = 0;


    unsigned int numStripsRequired = mZDim - 1;
    unsigned int numDegensRequired = 2 * (numStripsRequired - 1);
    unsigned int verticesPerStrip = 2 * mXDim;
    mTotalStripIndices = (verticesPerStrip * numStripsRequired) + numDegensRequired;
    mTotalIndices = (6 * (mXDim-1) * (mZDim-1));

    mRayTerrainIntersection = glm::vec3(0.0, 0.0, 0.0);
    mEditMode = 2.0f;
    mModifyRadius = 1.0f;
    mVPMatrix = camera->getVPMatrix();
    mInvViewMatrix = camera->getInvViewMatrix();
}

Terrain::~Terrain()
{
    glBindVertexArray(mVao);
    glDeleteBuffers(1, &mVbo);
    glDeleteBuffers(1, &mIbo);
    glDeleteVertexArrays(1, &mVao);
}

/*
 * Public control functions
 */
/* void Terrain::addNoise(std::vector<float> noiseValues) */
void Terrain::loadWater3DNoise( Noise* noise){
	Texture* texture = new Texture();
	texture->setData(noise->getInverseTextureData(), noise->getXDim(), noise->getYDim(), noise->getZDim());
	texture->linkTexture(mShaderProgram, "sTextureWaterDisp");
	texture->bind3D();
	texture->loadSkydome3DOptions();
}

void Terrain::loadWaterNormal3DNoise(Noise* noise)
{
	Texture* texture = new Texture();
	texture->setData(noise->getNormalMap(), noise->getXDim(), noise->getYDim(), noise->getZDim());
	texture->linkTexture(mShaderProgram, "sTextureWaterNormal");
	texture->bind3D();
	texture->loadWaterNormalOptions();
}

void Terrain::addNoise(const std::vector<float>& noiseValues)
{
    for(unsigned int i = 0; i < mNoiseValues.size(); i++)
        mNoiseValues.at(i) += noiseValues.at(i);
}
void Terrain::multiplyNoise(const std::vector<float>& noiseValues)
{
    for(unsigned int i = 0; i < mNoiseValues.size(); i++)
        mNoiseValues.at(i) *= noiseValues.at(i);
}

void Terrain::generateHeights()
{
    mNoise->generateNoiseImage();
    mNoiseValues = mNoise->getDataVector();
}

void Terrain::computeTerrain()
{
    generateHeights();
}

void Terrain::build()
{
    buildVBO();
    buildIBO();

    mWhiteNoiseTexture.bind();
    mWhiteNoiseTexture.loadNormalMapOptions();
    mWhiteNoiseTexture.linkTexture(mShaderProgram, "sWhiteNoise");

    if(mUseNormals)
        computeTerrainNormals();

    if(mUseNormalMap)
    {
        computeTerrainTangents();
        mNormalMapTexture.bind();
        mNormalMapTexture.loadNormalMapOptions();
        mNormalMapTexture.linkTexture(mShaderProgram, "sNormalMap");
        //normalMap.printCData();
    }
    setBuffers();
    /* buildDebug(); */
}

void Terrain::genHeightMapTexture()
{
    mUseHeightMapTexture = true;
    mHeightMapTexture.bind();
    mHeightMapTexture.setData(mNoise->getTextureData(), mNoise->getXDim(), mNoise->getYDim());
    mHeightMapTexture.loadHeightmapOptions();
    mHeightMapTexture.linkTexture(mShaderProgram, "sHeightMap");
}

void Terrain::linkHeightMapTexture(GLint shader)
{
    mHeightMapTexture.linkTexture(shader, "heightMap");
}

void Terrain::enableNormals()
{
    mUseNormals = true;
    mElementsPerVertex++;
    mFloatsPerVertex += 3;
}

void Terrain::enableNormalMap()
{
    enableNormals();
    mUseNormalMap = true;
    mElementsPerVertex += 3;
    mFloatsPerVertex += (3+3+2); //Tangent(3), Bitangent(3), UV(2)
}

void Terrain::drawReflection()
{
	glm::mat4 mMMatrix = glm::mat4(1.0f, 0.0f, 0.0f, 0.0f,
								0.0f, -1.0f, 0.0f, 0.0f,
								0.0f, 0.0f, 1.0f, 0.0f,
								0.0f, 0.0f, 0.0f, 1.0f);
	mVMatrix = mCamera->getViewMatrix();
	mVPMatrix = mCamera->getVPMatrix();
	mInvViewMatrix = mCamera->getInvViewMatrix();
	mCamPos = mCamera->getPosition();
	glUseProgram(mShaderProgram);
	glUniformMatrix4fv(muVPLocation, 1, GL_FALSE, value_ptr(mVPMatrix));
	glUniformMatrix4fv(muVLocation, 1, GL_FALSE, value_ptr(mVMatrix));
	glUniformMatrix4fv(glGetUniformLocation(mShaderProgram, "uMMatrix"), 1, GL_FALSE, value_ptr(mMMatrix));
	glUniformMatrix3fv(muInvViewLocation, 1, GL_FALSE, value_ptr(mInvViewMatrix));
	glUniform1i(muHeightMapTerrainRatioLocation, mHeightMapTerrainRatio);
	glUniform1i(muDrawGridLocation, mDrawGrid);
	glUniform3f(muRayTerrainIntersectionLocation, mRayTerrainIntersection.x, mRayTerrainIntersection.y, mRayTerrainIntersection.z);
	glUniform3f(muCamPosLocation, mCamPos.x, mCamPos.y, mCamPos.z);
	glUniform3f(muLightPosLocation, mLightPos.x, mLightPos.y, mLightPos.z);
	glUniform1f(muEditLocation, mEditMode);
	glUniform1f(muModifyRadius, mModifyRadius);
	glUniform1f(muTimeLocation, glfwGetTime());
	// Bind Attributes
	glBindVertexArray(mVao);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glUniform1i(muDrawGridLocation, mDrawGrid);
	/* glDrawElements(GL_TRIANGLE_STRIP, mTotalIndices, GL_UNSIGNED_INT, 0); */
	glDrawElements(GL_PATCHES, mTotalIndices, GL_UNSIGNED_INT, 0);

}

void Terrain::draw()
{
	glm::mat4 mMMatrix = glm::mat4(1.0f);
    mVMatrix = mCamera->getViewMatrix();
    mVPMatrix = mCamera->getVPMatrix();
    mInvViewMatrix = mCamera->getInvViewMatrix();
    mCamPos = mCamera->getPosition();
    glUseProgram(mShaderProgram);
    glUniformMatrix4fv(muVPLocation, 1, GL_FALSE, value_ptr(mVPMatrix));
    glUniformMatrix4fv(muVLocation, 1, GL_FALSE, value_ptr(mVMatrix));
    glUniformMatrix3fv(muInvViewLocation, 1, GL_FALSE, value_ptr(mInvViewMatrix));
    glUniformMatrix4fv(glGetUniformLocation(mShaderProgram, "uMMatrix"), 1, GL_FALSE, value_ptr(mMMatrix));
    glUniform1i(muHeightMapTerrainRatioLocation, mHeightMapTerrainRatio);
    glUniform1i(muDrawGridLocation, mDrawGrid);
    glUniform3f(muRayTerrainIntersectionLocation, mRayTerrainIntersection.x, mRayTerrainIntersection.y, mRayTerrainIntersection.z);
    glUniform3f(muCamPosLocation, mCamPos.x, mCamPos.y, mCamPos.z);
    glUniform3f(muLightPosLocation, mLightPos.x, mLightPos.y, mLightPos.z);
    glUniform1f(muEditLocation, mEditMode);
    glUniform1f(muModifyRadius, mModifyRadius);
    glUniform1f(muTimeLocation, glfwGetTime());
    // Bind Attributes
    glBindVertexArray(mVao);


    if(mDrawGrid == 0)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glUniform1i(muDrawGridLocation, mDrawGrid);
        /* glDrawElements(GL_TRIANGLE_STRIP, mTotalIndices, GL_UNSIGNED_INT, 0); */
        glDrawElements(GL_PATCHES, mTotalIndices, GL_UNSIGNED_INT, 0);
    }
    else if(mDrawGrid == 1)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glUniform1i(muDrawGridLocation, mDrawGrid);
        glDrawElements(GL_PATCHES, mTotalIndices, GL_UNSIGNED_INT, 0);
        /* glDrawElements(GL_LINE_STRIP, mTotalIndices, GL_UNSIGNED_INT, 0); */
    }
    else
    {
        glPolygonOffset(1, 1);
        glEnable(GL_POLYGON_OFFSET_FILL);

        mDrawGrid = 2;
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glUniform1i(muDrawGridLocation, mDrawGrid);
        glDrawElements(GL_PATCHES, mTotalIndices, GL_UNSIGNED_INT, 0);

        mDrawGrid = 3;
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glUniform1i(muDrawGridLocation, mDrawGrid);
        glDrawElements(GL_PATCHES, mTotalIndices, GL_UNSIGNED_INT, 0);
    }

    glBindVertexArray(0);
}

/*
 * Public getters / setters
 */
float Terrain::getHeightOfPosition(const glm::vec3& position) const
{
    glm::ivec2 indexCord0 = floor(getIndexCordFromTerrain(position));   // Bottom left
    glm::ivec2 indexCord3 = ceil(getIndexCordFromTerrain(position));    // Top right
    glm::ivec2 indexCord1 = glm::ivec2(indexCord0.x, indexCord3.y);     // Top left
    glm::ivec2 indexCord2 = glm::ivec2(indexCord3.x, indexCord0.y);     // Bottom right

    glm::vec3 cord0 = getTerrainPosition(indexCord0); // Bottom left
    glm::vec3 cord1 = getTerrainPosition(indexCord1); // Top left
    glm::vec3 cord2 = getTerrainPosition(indexCord2); // Bottom right
    glm::vec3 cord3 = getTerrainPosition(indexCord3); // Top right

    float dist0 = sqrt((position.x - cord0.x)*(position.x - cord0.x) + (position.z - cord0.z)*(position.z - cord0.z));
    float dist1 = sqrt((position.x - cord1.x)*(position.x - cord1.x) + (position.z - cord1.z)*(position.z - cord1.z));
    float dist2 = sqrt((position.x - cord2.x)*(position.x - cord2.x) + (position.z - cord2.z)*(position.z - cord2.z));
    float dist3 = sqrt((position.x - cord3.x)*(position.x - cord3.x) + (position.z - cord3.z)*(position.z - cord3.z));

    float interpolHeight = dist0 * cord0.y + dist1 * cord1.y + dist2 * cord2.y + dist3 * cord3.y;
    return interpolHeight;
}

float Terrain::getHeightMapValue(const glm::ivec2& coordinate) const
{
    return mNoiseValues.at((coordinate.y * (mNoise->getDimension()).x) + coordinate.x);
}

float Terrain::getTerrainHeight(const glm::ivec2& coordinate) const
{
    return mVertices.at(mFloatsPerVertex * ((coordinate.y * mXDim) + coordinate.x) + 1);
    /* glm::vec3 vertex = mVertices.at(mElementsPerVertex * ((coordinate.y * mXDim) + coordinate.x)); */
    /* return vertex.y; */
}

glm::vec3 Terrain::getTerrainPosition(const glm::ivec2& coordinate) const
{
    unsigned int index = ((mFloatsPerVertex * mTotalVertices) + (mFloatsPerVertex * ((coordinate.y * mXDim) + coordinate.x))) % (mFloatsPerVertex * mTotalVertices);
    return glm::vec3(mVertices.at(index), mVertices.at(index+1), mVertices.at(index+2));
    /* unsigned int index = mElementsPerVertex * ((coordinate.y * mXDim) + coordinate.x); */
    /* return mVertices.at(index); */
}

glm::vec2 Terrain::getIndexCordFromTerrain(const glm::vec3& position) const
{
    float x = (mXDim - 1) * ((position.x - mTerrainMinPos) / mTerrainPosRange);
    float z = (mZDim - 1) * (1 - ((-position.z - mTerrainMinPos) / mTerrainPosRange));
    if(x < 0.0f) x = 0.0f;
    if(x > mXDim-1) x = mXDim-1;
    if(z < 0.0f) z = 0.0f;
    if(z > mZDim-1) z = mZDim-1;
    return glm::vec2(x, z);
}



// Terrain computations
glm::vec3 Terrain::computePosition(unsigned int x, unsigned int z) const
{

    float xRatio = static_cast<float>(x) / static_cast<float>(mXDim -1);
    float zRatio = 1.0f - (static_cast<float>(z) / static_cast<float>(mZDim - 1));

    float xPosition = mTerrainMinPos + (xRatio * mTerrainPosRange);
    float yPosition = mNoiseValues.at(z * mHeightMapTerrainRatio * mNoise->getXDim() + x);
    float zPosition = mTerrainMinPos + (zRatio * mTerrainPosRange);
    yPosition = 0.5*pow(yPosition+0.5,5);

    return glm::vec3(xPosition, yPosition, -zPosition);
}

void Terrain::highlightRay(const glm::vec3& ray, unsigned int& mode)
{
    glm::vec3 curPos = mCamera->getPosition();
    mIntersectionCoords = getIntersectionPoint(curPos, ray);
    mEditMode = static_cast<float>(mode);
}

void Terrain::modifyHeight()
{
    /* updateArea(indexCord0, indexCord1, indexCord2, indexCord3); */
    if(mIntersectionCoords.x > -1)
        updateArea();
}

void Terrain::changeModifyRadius(const double& yoffset)
{
    if((yoffset < 0.0) && (mModifyRadius > 0.2f))
        mModifyRadius += 0.1f * static_cast<float>(yoffset);
    else if((yoffset > 0.0))
        mModifyRadius += 0.1f * static_cast<float>(yoffset);

}

// Noise getters
void Terrain::saveNoiseToFile(const char* filename) const
{
    mNoise->saveToFile(filename);
}

void Terrain::debug()
{

    /* std::cout << "verticex indices(" << mTotalVertices << "):" << std::endl; */
    /* for(unsigned int i = 0; i < mTotalVertices; i++) */
    /* { */
    /*     if((i % mXDim) == 0) std::cout << std::endl << i; */
    /*     else std::cout << "\t" << i; */
    /* } */
    /* std::cout << std::endl << std::endl; */

    /* std::cout << "indices(" << mTotalIndices << "):" << std::endl; */
    /* for(unsigned int i = 0; i < mTotalIndices; i++) */
    /* { */
    /*     if((i % 6) == 0) std::cout << std::endl << mIndices.at(i); */
    /*     else std::cout << ", " << mIndices.at(i); */
    /* } */
    /* std::cout << std::endl << std::endl; */

    /* std::cout << "vertices(" << mTotalVertices << "):" << std::endl; */
    /* glm::vec3 vec; */
    /* for(unsigned int i = 0; i < mTotalVertices; i += 2) */
    /* { */
    /*     vec = mVertices.at(i); */
    /*     printf("vertice:\t x = %5.2f, y = %5.2f, z = %5.2f\n" , vec.x, vec.y, vec.z); */
    /*     vec = mVertices.at(i+1); */
    /*     printf("normal:\t\t x = %5.2f, y = %5.2f, z = %5.2f\n" , vec.x, vec.y, vec.z); */
    /* } */

    /* std::cout << "noise values(" << mNoiseValues.size() << "):" << std::endl; */
    /* for(unsigned int i = 0; i < mTotalVertices; i++) */
    /* { */
    /*     printf("noise value = %5.2f\n" , mNoiseValues.at(i)); */
    /* } */

    std::cout << "verticex indices(" << mTotalVertices << "):" << std::endl;
    for(unsigned int i = 0; i < mTotalVertices; i++)
    {
        if((i % mXDim) == 0) std::cout << std::endl << i;
        else std::cout << "\t" << i;
    }
    std::cout << std::endl << std::endl;

    std::cout << "indices(" << mTotalIndices << "):" << std::endl;
    for(unsigned int i = 0; i < mTotalIndices; i++)
    {
        if((i % 6) == 0) std::cout << std::endl << mIndices.at(i);
        else std::cout << ", " << mIndices.at(i);
    }
    std::cout << std::endl << std::endl;

    std::cout << "vertices(" << mTotalVertices << "):" << std::endl;
    for(unsigned int i = 0; i < mVertices.size(); i += mFloatsPerVertex)
    {
        printf("vertice:\t x = %5.2f, y = %5.2f, z = %5.2f\n" , mVertices.at(i+0), mVertices.at(i+1), mVertices.at(i+2) );
        printf("normal:\t\t x = %5.2f, y = %5.2f, z = %5.2f\n" , mVertices.at(i+3), mVertices.at(i+4), mVertices.at(i+5) );
    }

    /* std::cout << "noise values(" << mNoiseValues.size() << "):" << std::endl; */
    /* for(unsigned int i = 0; i < mTotalVertices; i++) */
    /* { */
    /*     printf("noise value = %5.2f\n" , mNoiseValues.at(i)); */
    /* } */


    std::cout << "vertex attributes(" << mVertices.size() << "):" << std::endl;
    for(unsigned int i = 0; i < mVertices.size(); i += mFloatsPerVertex)
    {
        printf("x = %5.2f, y = %5.2f, z = %5.2f, nx = %5.2f, ny = %5.2f, nz = %5.2f, tx = %5.2f, ty = %5.2f, tz = %5.2f, bx = %5.2f, by = %5.2f, bz = %5.2f, uvx = %5.2f, uvz = %5.2f\n" , mVertices.at(i+0), mVertices.at(i+1), mVertices.at(i+2), mVertices.at(i+3), mVertices.at(i+4), mVertices.at(i+5), mVertices.at(i+6), mVertices.at(i+7), mVertices.at(i+8), mVertices.at(i+9), mVertices.at(i+10), mVertices.at(i+11), mVertices.at(i+12), mVertices.at(i+13) );
    }
}

/*
 *
 * Private methods
 *
 */

// Getters
unsigned int Terrain::getSize() const
{
    return mXDim * mZDim;
}

unsigned int Terrain::getVerticeNumber() const
{
    return mTotalVertices;
}

unsigned int Terrain::getIndicesPerRow() const
{
    return 2 * mXDim + 2;
}

unsigned int Terrain::getDegensRequired() const
{
    return (mXDim - 1) * 2;
}

unsigned int Terrain::getIndexLeftPosition(unsigned int index) const
{
    /* return (index - 3) % (3*mTotalVertices); */
    return ((mFloatsPerVertex * mTotalVertices) + index - mFloatsPerVertex) % (mFloatsPerVertex * mTotalVertices);
    /* return ((mElementsPerVertex * mTotalVertices) + index - mElementsPerVertex) % (mElementsPerVertex * mTotalVertices); */
}

unsigned int Terrain::getIndexRightPosition(unsigned int index) const
{
    /* return (index + 3) % (3*mTotalVertices); */
    return (index + mFloatsPerVertex) % (mFloatsPerVertex * mTotalVertices);
    /* return (index + mElementsPerVertex) % (mElementsPerVertex * mTotalVertices); */
}

unsigned int Terrain::getIndexTopPosition(unsigned int index) const
{
    /* return (index - (3 * mXDim)) % (3*mTotalVertices); */
    return ((mFloatsPerVertex * mTotalVertices) + (index - (mFloatsPerVertex * mXDim))) % (mFloatsPerVertex * mTotalVertices);
    /* return ((mElementsPerVertex * mTotalVertices) + (index - (mElementsPerVertex * mXDim))) % (mElementsPerVertex * mTotalVertices); */
}

unsigned int Terrain::getIndexBottomPosition(unsigned int index) const
{
    /* return (index + (3 * mXDim)) % (3*mTotalVertices); */
    return (index + (mFloatsPerVertex * mXDim)) % (mFloatsPerVertex * mTotalVertices);
    /* return (index + (mElementsPerVertex * mXDim)) % (mElementsPerVertex * mTotalVertices); */
}

unsigned int Terrain::getIndexTopRightPosition(unsigned int index) const
{
    return getIndexRightPosition(getIndexTopPosition(index));
}

unsigned int Terrain::getIndexBottomLeftPosition(unsigned int index) const
{
    return getIndexLeftPosition(getIndexBottomPosition(index));
}



// Terrain computation / generation
void Terrain::computeTerrainPositions()
{
    glm::vec3 pos;
    for(unsigned int z = 0; z < mZDim; z++)
    {
        for(unsigned int x = 0; x < mXDim; x++)
        {
            pos = computePosition(x, z);
            mVertices.at(mFloatsPerVertex * ((z * mXDim) + x)) = pos.x;
            mVertices.at(mFloatsPerVertex * ((z * mXDim) + x) + 1) = pos.y;
            mVertices.at(mFloatsPerVertex * ((z * mXDim) + x) + 2) = pos.z;
            /* mVertices.at(mElementsPerVertex * ((z * mXDim) + x)) = computePosition(x, z); */
        }
    }
}

void Terrain::computeTerrainNormals()
{
    computeTerrainNormals(0, mVertices.size());
}

void Terrain::computeTerrainNormals(unsigned int min, unsigned int max)
{

    /* for(std::vector<glm::vec3>::iterator normal = mVertices.begin(); normal != mVertices.end(); normal += 2) */
    glm::vec3 c, l, r, t, b, tr, bl;
    glm::vec3 normalA, normalB, normalC, normalD, normalE, normalF, vNormal;
    unsigned int index;
    /* Triangle A, B, c, D, E, F; */
    for(unsigned int i = min; i < max; i += mFloatsPerVertex)
    /* for(unsigned int i = 0; i < mVertices.size(); i += mElementsPerVertex) */
    {
        c = glm::vec3(mVertices.at(i), mVertices.at(i+1), mVertices.at(i+2));
        index = getIndexLeftPosition(i);
        l = glm::vec3(mVertices.at(index), mVertices.at(index+1), mVertices.at(index+2));
        index = getIndexRightPosition(i);
        r = glm::vec3(mVertices.at(index), mVertices.at(index+1), mVertices.at(index+2));
        index = getIndexTopPosition(i);
        t = glm::vec3(mVertices.at(index), mVertices.at(index+1), mVertices.at(index+2));
        index = getIndexBottomPosition(i);
        b = glm::vec3(mVertices.at(index), mVertices.at(index+1), mVertices.at(index+2));
        index = getIndexTopRightPosition(i);
        tr = glm::vec3(mVertices.at(index), mVertices.at(index+1), mVertices.at(index+2));
        index = getIndexBottomLeftPosition(i);
        bl = glm::vec3(mVertices.at(index), mVertices.at(index+1), mVertices.at(index+2));

        /* c = mVertices.at(i); */
        /* l = mVertices.at(getIndexLeftPosition(i)); */
        /* r = mVertices.at(getIndexRightPosition(i)); */
        /* t = mVertices.at(getIndexTopPosition(i)); */
        /* b = mVertices.at(getIndexBottomPosition(i)); */
        /* tr = mVertices.at(getIndexTopRightPosition(i)); */
        /* bl = mVertices.at(getIndexBottomLeftPosition(i)); */


        // Triangle, counter clockwise vertices
        /* Triangle A = {c, t, l}; */
        /* Triangle B = {c, tr, t}; */
        /* Triangle C = {c, r, tr}; */
        /* Triangle D = {c, b, r}; */
        /* Triangle E = {c, bl, b}; */
        /* Triangle F = {c, l, bl}; */
        // Triangle, clockwise vertices
        Triangle A = {c, l, t};
        Triangle B = {c, t, tr};
        Triangle C = {c, tr,r};
        Triangle D = {c, r, b};
        Triangle E = {c, b, bl};
        Triangle F = {c, bl,l};

        normalA = computeTriangleNormal(A);
        normalB = computeTriangleNormal(B);
        normalC = computeTriangleNormal(C);
        normalD = computeTriangleNormal(D);
        normalE = computeTriangleNormal(E);
        normalF = computeTriangleNormal(F);

        vNormal = computeVertexNormal(normalA, normalB, normalC, normalD, normalE, normalF);

        mVertices.at(i+3) = vNormal.x;
        mVertices.at(i+4) = vNormal.y;
        mVertices.at(i+5) = vNormal.z;

        /* mVertices.at(i+1) = vNormal; */

    }
}

void Terrain::computeTerrainTangents()
{
    computeTerrainTangents(0, mVertices.size());
}

void Terrain::computeTerrainTangents(unsigned int min, unsigned int max)
{
    float dUVx = 1.0f / mXDim;
    float dUVz = 1.0f / mZDim;
    for(unsigned int z = 0; z < mZDim; z++)
    {
        for(unsigned int x = 0; x < mXDim; x++)
        {
            mVertices.at(mFloatsPerVertex * ((z * mXDim) + x) + 12) = x * dUVx;
            mVertices.at(mFloatsPerVertex * ((z * mXDim) + x) + 13) = z * dUVz;
        }
    }

    unsigned int i0, i1, i2;
    glm::vec3 v0, v1, v2;
    glm::vec2 uv0, uv1, uv2;
    glm::vec3 tangent, bitangent;
    glm::vec3 deltaPos0, deltaPos1;
    float r = 1.0f;
    for(unsigned int i = 0; i < mIndices.size(); i += 3)
    {

        i0 = mIndices.at(i);
        i2 = mIndices.at(i+1);
        i1 = mIndices.at(i+2);
        /* std::cout << "Indices: i0 = " << i0 << ", i1 = " << i1 << ", i2 = " << i2 << ", mFloatsPerVertex = " << mFloatsPerVertex << std::endl; */
        v0 = glm::vec3(mVertices.at(i0 * mFloatsPerVertex), mVertices.at(i0 * mFloatsPerVertex + 1), mVertices.at(i0 * mFloatsPerVertex + 2));
        v1 = glm::vec3(mVertices.at(i1 * mFloatsPerVertex), mVertices.at(i1 * mFloatsPerVertex + 1), mVertices.at(i1 * mFloatsPerVertex + 2));
        v2 = glm::vec3(mVertices.at(i2 * mFloatsPerVertex), mVertices.at(i2 * mFloatsPerVertex + 1), mVertices.at(i2 * mFloatsPerVertex + 2));

        deltaPos0 = glm::vec3(v1.x - v0.x, v1.y - v0.y, v1.z - v0.z);
        deltaPos1 = glm::vec3(v2.x - v0.x, v2.y - v0.y, v2.z - v0.z);

        tangent = (deltaPos0 * dUVz - deltaPos1 * dUVz) * r;
        bitangent = (deltaPos1 * dUVx - deltaPos0 * dUVx) * r;
        /* v1 = glm::vec3(mVertices.at(i), mVertices.at(i+1), mVertices(i+2)); */
        mVertices.at(i0*mFloatsPerVertex + 6) = tangent.x;
        mVertices.at(i0*mFloatsPerVertex + 7) = tangent.y;
        mVertices.at(i0*mFloatsPerVertex + 8) = tangent.z;
        mVertices.at(i1*mFloatsPerVertex + 6) = tangent.x;
        mVertices.at(i1*mFloatsPerVertex + 7) = tangent.y;
        mVertices.at(i1*mFloatsPerVertex + 8) = tangent.z;
        mVertices.at(i2*mFloatsPerVertex + 6) = tangent.x;
        mVertices.at(i2*mFloatsPerVertex + 7) = tangent.y;
        mVertices.at(i2*mFloatsPerVertex + 8) = tangent.z;

        mVertices.at(i0*mFloatsPerVertex + 9) = bitangent.x;
        mVertices.at(i0*mFloatsPerVertex + 10) = bitangent.y;
        mVertices.at(i0*mFloatsPerVertex + 11) = bitangent.z;
        mVertices.at(i1*mFloatsPerVertex + 9) = bitangent.x;
        mVertices.at(i1*mFloatsPerVertex + 10) = bitangent.y;
        mVertices.at(i1*mFloatsPerVertex + 11) = bitangent.z;
        mVertices.at(i2*mFloatsPerVertex + 9) = bitangent.x;
        mVertices.at(i2*mFloatsPerVertex + 10) = bitangent.y;
        mVertices.at(i2*mFloatsPerVertex + 11) = bitangent.z;

        /* std::cout << "tangent: x = " << tangent.x << ", y = " << tangent.y << ", z = " << tangent.z << std::endl; */
        /* std::cout << "bitangent: x = " << bitangent.x << ", y = " << bitangent.y << ", z = " << bitangent.z << std::endl; */
    }

}

void Terrain::buildVBO()
{
    // Position(3), Normal(3), Tangent(3), Bitangent(3), UV(2)
    /* mVertices.resize(mTotalVertices * mElementsPerVertex); */

    mVertices.resize(mTotalVertices * mFloatsPerVertex);
    computeTerrainPositions();

}

void Terrain::buildIBO()
{

    mIndices.resize(mTotalIndices);
    int offset = 0;
    for(unsigned int z = 0; z < mZDim - 1; z++)
    {
        for(unsigned int x = 0; x < mXDim - 1; x++)
        {
            mIndices.at(offset++) = ((z * mXDim) + x);
            mIndices.at(offset++) = (((z+1) * mXDim) + x);
            mIndices.at(offset++) = ((z * mXDim) + x + 1);

            mIndices.at(offset++) = ((z * mXDim) + x + 1);
            mIndices.at(offset++) = (((z+1) * mXDim) + x);
            mIndices.at(offset++) = (((z+1) * mXDim) + x + 1);
        }

    }


    // Stripe indices
    /* for(int z = 0; z < mZDim - 1; z++) */
    /* { */
    /*     if(z > 0) */
    /*         mIndices[offset++] = (z * mXDim); */

    /*     for(int x = 0; x < mXDim; x++) */
    /*     { */
    /*         mIndices[offset++] = ((z * mXDim) + x); */
    /*         mIndices[offset++] = (((z+1) * mXDim) + x); */
    /*     } */

    /*     if(z < mZDim - 2) */
    /*         mIndices[offset++] = (((z+1) * mXDim) + (mXDim - 1)); */

    /* } */

}

void Terrain::buildDebug()
{

/*     mTotalVertices = 9; */
/*     mTotalIndices = 3; */

/*     mVertices = new GLfloat[mTotalVertices]{0, 0, 0, 0, 1, 0, 1, 1, 0}; */
/*     mIndices = new GLint[mTotalVertices]; */
/*     //Generate & bind ibo */
/*     mIndices[0] = 0; */
/*     mIndices[1] = 1; */
/*     mIndices[2] = 2; */

}

unsigned int Terrain::getIndexFromPosition( const glm::ivec2& pos ) const
{
    return (mFloatsPerVertex * ((pos.y * mXDim) + pos.x));
}

float Terrain::getIndexDistances(const glm::ivec2& p1, const glm::ivec2& p2) const
{

    glm::vec3 t1 = getTerrainPosition(p1);
    glm::vec3 t2 = getTerrainPosition(p2);
    return glm::distance(glm::vec2(t1.x, t1.z), (glm::vec2(t2.x, t2.z)));
}

void Terrain::updateArea( )
{

    /* std::cout << "indices for update area: x = " << ip.x << ", z = " << ip.y << std::endl; */
    /* std::cout << "update radius = " << mModifyRadius << std::endl; */
    int hg = 0, hl = 0, vg = 0, vl = 0;
    for(unsigned int x = 1; x < mXDim; x++)
    {
        if(getIndexDistances(mIntersectionCoords, glm::ivec2(mIntersectionCoords.x + x, mIntersectionCoords.y)) > mModifyRadius) break;
        hg++;
    }
    for(unsigned int x = 1; x < mXDim; x++)
    {
        if(getIndexDistances(mIntersectionCoords, glm::ivec2(mIntersectionCoords.x - x, mIntersectionCoords.y)) > mModifyRadius) break;
        hl++;
    }

    for(unsigned int z = 1; z < mZDim; z++)
    {
        if(getIndexDistances(mIntersectionCoords, glm::ivec2(mIntersectionCoords.x, mIntersectionCoords.y + z)) > mModifyRadius) break;
        vg++;
    }
    for(unsigned int z = 1; z < mZDim; z++)
    {
        if(getIndexDistances(mIntersectionCoords, glm::ivec2(mIntersectionCoords.x, mIntersectionCoords.y - z)) > mModifyRadius) break;
        vl++;
    }

    if(mIntersectionCoords.x + hg >= static_cast<int>(mXDim))   hg = mXDim - mIntersectionCoords.x - 1;
    if(mIntersectionCoords.x - hl < 0)                          hl = mIntersectionCoords.x;
    if(mIntersectionCoords.y + vg >= static_cast<int>(mZDim))   vg = mZDim - mIntersectionCoords.y - 1;
    if(mIntersectionCoords.y - vl < 0)                          vl = mIntersectionCoords.y;

    /* std::cout << "mIntersectionCoords: x = " << mIntersectionCoords.x << ", z = " << mIntersectionCoords.y << std::endl; */
    /* std::cout << "hl = " << hl << ", hg = " << hg << ", vl = " << vl << ", vg = " << vg << std::endl; */
    /* std::cout << "mModifyRadius = " << mModifyRadius << std::endl; */
    float mode = ((mEditMode == 0.0) ? -0.1f : 0.1f);
    float dist, scale;
    for(int i = (mIntersectionCoords.y - vl); i <= (mIntersectionCoords.y + vg); i++)
    {
        for(int j = (mIntersectionCoords.x - hl); j <= (mIntersectionCoords.x + hg); j++)
        {
            dist = getIndexDistances(mIntersectionCoords, glm::ivec2(j, i));
            /* std::cout << "Updating position: x = " << j << ", z = " << i << ", with height = " << scale << std::endl; */
            if(dist < mModifyRadius)
            {
                scale = mode/(mModifyRadius*mModifyRadius) * (dist+mModifyRadius)*(dist-mModifyRadius);
                mVertices.at(getIndexFromPosition(glm::ivec2(j, i)) + 1) += scale;
            }
            /* else */
            /* { */
            /*     std::cout << "NOT UPDATE position: x = " << j << ", z = " << i << ", with height = " << scale << std::endl; */
            /* } */
        }
    }

    /* unsigned int mi = getIndexFromPosition(mIntersectionCoords); */
    /* unsigned int t = getIndexTopPosition(i); */
    /* unsigned int b = getIndexBottomPosition(i); */
    /* unsigned int l = getIndexLeftPosition(i); */
    /* unsigned int r = getIndexRightPosition(i); */
    /* std::cout << "(i,t,b,l,r) = (" << i << ", " << t << ", " << b << ", " << l << ", " << r << ")" << std::endl; */
    /* unsigned int offset = t; */
    /* unsigned int size = mFloatsPerVertex + (b - t); */

    unsigned int offset = getIndexFromPosition(glm::ivec2(mIntersectionCoords.x-hl, mIntersectionCoords.y-vl));
    unsigned int offsetEnd = getIndexFromPosition(glm::ivec2(mIntersectionCoords.x+hg, mIntersectionCoords.y+vg));
    if(offsetEnd >= mVertices.size()) offsetEnd = mVertices.size();
    unsigned int size = (mFloatsPerVertex + (offsetEnd - offset));
    /* std::cout << "buffer offset = " << offset << ", buffer end = " << offsetEnd << ", size = " << size << std::endl; */
    if(mUseNormals) computeTerrainNormals(offset, offset + size);
    if(mUseNormalMap) computeTerrainTangents(offset, offset + size);


    // Set new hights
    /* mVertices.at(i+1) += 2.5f; */
    /* mVertices.at(t+1) += 1.25f; */
    /* mVertices.at(b+1) += 1.25f; */
    /* mVertices.at(l+1) += 1.25f; */
    /* mVertices.at(r+1) += 1.25f; */

    // Fill update buffer
    std::vector<float> newData;
    newData.resize(size);
    for(unsigned int i = 0; i < size; i++)
    {
        newData.at(i) = mVertices.at(offset + i);
    }


    /* newData[1] += 0.5f; */
    /* newData[relTlOffset + 1] += 0.5f; */
    /* newData[relBrOffset + 1] += 0.5f; */
    /* newData[relTrOffset + 1] += 0.5f; */


    /* int offset  = mFloatsPerVertex * ((coords.y * mXDim) + coords.x); */
    /* int size    = 60 * mFloatsPerVertex; */

    updateVertexBuffer(offset, size, newData.data());

}

void Terrain::updateVertexBuffer(unsigned int offset, unsigned int size, float* newVertexData)
{

    // Bind Attributes

    glBindBuffer(GL_ARRAY_BUFFER, mVbo);
    /* void *vertexData = glMapBufferRange(GL_ARRAY_BUFFER, 0, 3, GL_MAP_WRITE_BIT | GL_MAP_UNSYNCHRONIZED_BIT); */
    /* float* vertexBuffer = reinterpret_cast<float*>(glMapBufferRange(GL_ARRAY_BUFFER, offset*sizeof(GLfloat), size*sizeof(GLfloat), GL_MAP_WRITE_BIT)); */
    /* void* vertexBuffer = (glMapBufferRange(GL_ARRAY_BUFFER, offset*sizeof(GLfloat), size*sizeof(GLfloat), GL_MAP_WRITE_BIT)); */
    /* float* vertexData = reinterpret_cast<float*>(vertexBuffer); */
    float* vertexData = reinterpret_cast<float*>(glMapBufferRange(GL_ARRAY_BUFFER, offset*sizeof(GLfloat), size*sizeof(GLfloat), GL_MAP_WRITE_BIT));

    // Fill Buffer
    for(unsigned int i = 0; i < size; i++)
    {
        vertexData[i] = newVertexData[i];
    }

    // Unmap
    glUnmapBuffer(GL_ARRAY_BUFFER);

    /* glBufferData(GL_ARRAY_BUFFER, mVertices.size() * sizeof(GLfloat), mVertices.data(), GL_STATIC_DRAW); */
    /* glBindVertexArray(0); */

}

void Terrain::setBuffers()
{
    //Unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glUseProgram(mShaderProgram);

    // Get Attrib location
    GLint terrainPosAttrib = glGetAttribLocation(mShaderProgram, "vPosition");
    GLint terrainNormalAttrib = glGetAttribLocation(mShaderProgram, "vNormal");
    GLint terrainTangentAttrib = glGetAttribLocation(mShaderProgram, "vTangent");
    GLint terrainBitangentAttrib = glGetAttribLocation(mShaderProgram, "vBitangent");
    GLint terrainUVAttrib = glGetAttribLocation(mShaderProgram, "vUV");
    muVPLocation = glGetUniformLocation(mShaderProgram, "uVPMatrix");
    muVLocation = glGetUniformLocation(mShaderProgram, "uVMatrix");
    muInvViewLocation = glGetUniformLocation(mShaderProgram, "uInvViewMatrix");
    muHeightMapTerrainRatioLocation = glGetUniformLocation(mShaderProgram, "uHeightMapTerrainRatio");
    muDrawGridLocation = glGetUniformLocation(mShaderProgram, "uDrawGrid");
    muRayTerrainIntersectionLocation = glGetUniformLocation(mShaderProgram, "uRayTerrainIntersection");
    muEditLocation = glGetUniformLocation(mShaderProgram, "uEditMode");
    muModifyRadius = glGetUniformLocation(mShaderProgram, "uModifyRadius");
    muCamPosLocation = glGetUniformLocation(mShaderProgram, "uCamPos");
    muLightPosLocation = glGetUniformLocation(mShaderProgram, "uLightPos");
    muTimeLocation = glGetUniformLocation(mShaderProgram, "uTime");

    //Generate & bind vao
    glGenVertexArrays(1, &mVao);
    glBindVertexArray(mVao);

    //Generate & bind vbo
    glGenBuffers(1, &mVbo);
    /* glBindBuffer(GL_COPY_WRITE_BUFFER, mVbo); */
    /* glBufferData(GL_COPY_WRITE_BUFFER, mVertices.size() * sizeof(GLfloat), mVertices.data(), GL_STATIC_DRAW); */
    glBindBuffer(GL_ARRAY_BUFFER, mVbo);
    glBufferData(GL_ARRAY_BUFFER, mVertices.size() * sizeof(GLfloat), mVertices.data(), GL_STATIC_DRAW);
    /* glBufferData(GL_ARRAY_BUFFER, mVertices.size() * sizeof(glm::vec3), &mVertices[0].x, GL_STATIC_DRAW); */

    // First all positions, then all normals
    glEnableVertexAttribArray(terrainPosAttrib);
    glVertexAttribPointer(terrainPosAttrib, 3, GL_FLOAT, GL_FALSE, mFloatsPerVertex * sizeof(GLfloat), 0);
    glPatchParameteri(GL_PATCH_VERTICES, 3);
    /* glVertexAttribPointer(terrainPosAttrib, 3, GL_FLOAT, GL_FALSE, mElementsPerVertex * sizeof(glm::vec3), 0); */
    if(mUseNormals)
    {
        glEnableVertexAttribArray(terrainNormalAttrib);
        /* glVertexAttribPointer(terrainNormalAttrib, 3, GL_FLOAT, GL_FALSE, 12, reinterpret_cast<const GLvoid*>(mFloatsPerVertex/2 * mTotalVertices * sizeof(GLfloat))); */
        glVertexAttribPointer(terrainNormalAttrib, 3, GL_FLOAT, GL_FALSE, mFloatsPerVertex * sizeof(GLfloat), reinterpret_cast<const GLvoid*>(3 * sizeof(GLfloat)));
        /* glVertexAttribPointer(terrainNormalAttrib, 3, GL_FLOAT, GL_FALSE, mElementsPerVertex * sizeof(glm::vec3), reinterpret_cast<const GLvoid*>(sizeof(glm::vec3))); */
    }
    if(mUseNormalMap)
    {
        // Tangents
        glEnableVertexAttribArray(terrainTangentAttrib);
        glVertexAttribPointer(terrainTangentAttrib, 3, GL_FLOAT, GL_FALSE, mFloatsPerVertex * sizeof(GLfloat), reinterpret_cast<const GLvoid*>(6 * sizeof(GLfloat)));

        // Bitangents
        glEnableVertexAttribArray(terrainBitangentAttrib);
        glVertexAttribPointer(terrainBitangentAttrib, 3, GL_FLOAT, GL_FALSE, mFloatsPerVertex * sizeof(GLfloat), reinterpret_cast<const GLvoid*>(9 * sizeof(GLfloat)));

        // UV
        glEnableVertexAttribArray(terrainUVAttrib);
        glVertexAttribPointer(terrainUVAttrib, 2, GL_FLOAT, GL_FALSE, mFloatsPerVertex * sizeof(GLfloat), reinterpret_cast<const GLvoid*>(12 * sizeof(GLfloat)));

    }

    //Generate & bind ibo
    glGenBuffers(1, &mIbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIbo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mIndices.size() * sizeof(GLint), mIndices.data(), GL_STATIC_DRAW);

    //Unbind
    glBindVertexArray(0);
    /* delete mVertices; */
    /* delete mIndices; */

}

glm::vec3 Terrain::computeTriangleNormal(Terrain::Triangle &t)
{
    return glm::cross(glm::vec3(t.c - t.a), glm::vec3(t.b - t.a));
}

glm::vec3 Terrain::computeVertexNormal(glm::vec3 v1, glm::vec3 v2, glm::vec3 v3, glm::vec3 v4, glm::vec3 v5, glm::vec3 v6 )
{
    return glm::normalize(v1 + v2 + v3 + v4 + v5 + v6);
}


glm::ivec2 Terrain::getIntersectionPoint(const glm::vec3& position, const glm::vec3& ray)
{
    glm::vec2 is = getIndexCordFromTerrain(position);
    glm::ivec2 sp = glm::ivec2(round(is.x), round(is.y));
    float h = position.y;
    /* std::cout << "camera position: x = " << position.x << ", y = " << position.y << ", z = " << position.z << "\t dy = " << dy << std::endl; */

    unsigned int x1 = sp.x;
    unsigned int x2 = ((ray.x < 0) ? 0 : mXDim);
    int deltaX = round(mXDim * (ray.x));
    int dx((deltaX > 0) - (deltaX < 0));

    unsigned int z1 = sp.y;
    unsigned int z2 = ((ray.z < 0) ? 0 : mZDim);
    int deltaZ = round(mZDim * (ray.z));
    int dz((deltaZ > 0) - (deltaZ < 0));

    deltaX = std::abs(deltaX) << 1;
    deltaZ = std::abs(deltaZ) << 1;

    glm::vec3 tc;
    /* std::cout << "x1 = " << x1 << ", x2 = " << x2 << ", dX = " << deltaX << ", dx = " << dx << "\tz1 = " << z1 << ", z2 = " << z2 << ", dZ = " << deltaZ <<", dz = " << dz << std::endl; */
    /* std::cout << "trace" << std::endl; */
    if(deltaX < deltaZ)
    {

        int error(deltaX - (deltaZ >> 1));
        /* while ((z1 != z2) && (z1 < mZDim) && (z1 > 0)) */
        while(z1 != z2)
        {
            tc = getTerrainPosition(glm::ivec2(x1, z1));
            h = position.y + glm::distance(tc, position)*ray.y;
            /* std::cout << "terrain indices: x = " << x1 << ", z = " << z1 << "\t ray height = " << h << std::endl; */
            /* std::cout << "terrain position: x = " << tc.x << ", y = " << tc.y << ", z = " << tc.z << std::endl; */
            /* std::cout << "diffs: dx = " << tc.x-position.x << ", dy = " << tc.y-position.y << ", dz = " << tc.z-position.z << ", dh = " << glm::length(glm::vec2(tc.x-position.x, tc.z-position.z))*ray.y << std::endl; */
            if(tc.y >= h)
            {
                mRayTerrainIntersection = tc;
                /* std::cout << "INTERSECTION" << std::endl; */
                return glm::ivec2(x1, z1);
            }
            if ((error >= 0) && (error || (dz > 0)))
            {
                error -= deltaZ;
                x1 += dx;
            }

            error += deltaX;
            z1 += dz;

        }

    }
    else
    {

        int error(deltaZ - (deltaX >> 1));

        while(x1 != x2)
        {

            tc = getTerrainPosition(glm::ivec2(x1, z1));
            h = position.y + glm::distance(tc, position)*ray.y;
            /* std::cout << "terrain indices: x = " << x1 << ", z = " << z1 << "\t ray height = " << h << std::endl; */
            /* std::cout << "terrain position: x = " << tc.x << ", y = " << tc.y << ", z = " << tc.z << std::endl; */
            /* std::cout << "diffs: dx = " << tc.x-position.x << ", dy = " << tc.y-position.y << ", dz = " << tc.z-position.z << ", dh = " << glm::length(glm::vec2(tc.x-position.x, tc.z-position.z))*ray.y << std::endl; */
            if(tc.y >= h)
            {
                mRayTerrainIntersection = tc;
                /* std::cout << "INTERSECTION" << std::endl; */
                return glm::ivec2(x1, z1);
            }
            if ((error >= 0) && (error || (dx > 0)))
            {
                error -= deltaX;
                z1 += dz;
            }

            error += deltaZ;
            x1 += dx;

        }
    }

    /* std::cout << "NO INTERSECTION" << std::endl; */
    return glm::ivec2(-1, -1);
}

