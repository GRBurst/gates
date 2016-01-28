#include "Terrain.h"

Terrain::Terrain(GLint shaderProgram, unsigned int xDim, unsigned int zDim, Camera *const camera, Noise *const noise)
    : mXDim(xDim)
    , mZDim(zDim)
    , mTotalVertices(xDim * zDim)
    , mHeightMapTexture()
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
    mVPMatrix = camera->getVPMatrix();
    mInvViewMatrix = camera->getInvViewMatrix();
}

Terrain::~Terrain()
{
}

/*
 * Public control functions
 */
void Terrain::generateHeights()
{
    mNoise->generateNoiseImage();
    mNoiseValues = mNoise->getDataVector();
}

void Terrain::computeTerrain()
{
    generateHeights();
    build();
    setBuffers();
}

void Terrain::genHeightMapTexture()
{
    mUseHeightMapTexture = true;
    mHeightMapTexture.bind();
    mHeightMapTexture.setData(mNoise->getTextureData(), mNoise->getXDim(), mNoise->getYDim());
    mHeightMapTexture.loadHeightmapOptions();
    mHeightMapTexture.linkTexture(mShaderProgram, "heightMap");
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
    mUseNormalMap = true;
    mElementsPerVertex += 3;
    mFloatsPerVertex += (3+3+2); //Tangent(3), Bitangent(3), UV(2)
}

void Terrain::draw()
{
    mVPMatrix = mCamera->getVPMatrix();
    mInvViewMatrix = mCamera->getInvViewMatrix();
    glUseProgram(mShaderProgram);
    glUniformMatrix4fv(muVPLocation, 1, GL_FALSE, value_ptr(mVPMatrix));
    glUniformMatrix3fv(muInvViewLocation, 1, GL_FALSE, value_ptr(mInvViewMatrix));
    glUniform1i(muHeightMapTerrainRatioLocation, mHeightMapTerrainRatio);
    glUniform1i(muDrawGridLocation, mDrawGrid);
    glUniform3f(muRayTerrainIntersectionLocation, mRayTerrainIntersection.x, mRayTerrainIntersection.y, mRayTerrainIntersection.z);
    glUniform3f(muCamPos, mCamPos.x, mCamPos.y, mCamPos.z);

    // Bind Attributes
    glBindVertexArray(mVao);

    if(mDrawGrid == 0)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glUniform1i(muDrawGridLocation, mDrawGrid);
        /* glDrawElements(GL_TRIANGLE_STRIP, mTotalIndices, GL_UNSIGNED_INT, 0); */
        glDrawElements(GL_TRIANGLES, mTotalIndices, GL_UNSIGNED_INT, 0);
    }
    else if(mDrawGrid == 1)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glUniform1i(muDrawGridLocation, mDrawGrid);
        glDrawElements(GL_TRIANGLES, mTotalIndices, GL_UNSIGNED_INT, 0);
        /* glDrawElements(GL_LINE_STRIP, mTotalIndices, GL_UNSIGNED_INT, 0); */
    }
    else
    {
        glPolygonOffset(1, 1);
        glEnable(GL_POLYGON_OFFSET_FILL);

        mDrawGrid = 2;
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glUniform1i(muDrawGridLocation, mDrawGrid);
        glDrawElements(GL_TRIANGLES, mTotalIndices, GL_UNSIGNED_INT, 0);

        mDrawGrid = 3;
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glUniform1i(muDrawGridLocation, mDrawGrid);
        glDrawElements(GL_TRIANGLES, mTotalIndices, GL_UNSIGNED_INT, 0);
    }

    glBindVertexArray(0);
}

/*
 * Public getters / setters
 */
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
    unsigned int index = mFloatsPerVertex * ((coordinate.y * mXDim) + coordinate.x);
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

void Terrain::getRayTerrainIntersection(glm::vec3& ray)
{

    float dist0, dist1, dist2, dist3;
    glm::vec3 cord0, cord1, cord2, cord3;
    glm::ivec2 floorVals, ceilVals;
    glm::ivec2 indexCord, indexCord0, indexCord1, indexCord2, indexCord3;

    glm::vec3 curPos = mCamera->getPosition();
    float interpolHeight = curPos.y;
    float scale = 0.1f;
    int bisectState = 0;
    glm::vec3 tmpRay = scale * ray;
    /* std::cout << "camPosition: x = " << curPos.x << ", y = " << curPos.y << ", z = " << curPos.z << std::endl; */
    /* std::cout << "ray trace: " << std::endl; */
    while( (curPos.y > -1.0f) && (curPos.y < 25.0f))
    {
        if(curPos.y < interpolHeight )
        {
            /* std::cout << "INTERSECTION at hight = " << interpolHeight << std::endl; */
            break;
        }

        /* curPos += scale * tmpRay; */
        curPos += tmpRay;

        /* indexCord = getIndexCordFromTerrain(curPos); */
        /* floorVals = floor(getIndexCordFromTerrain(curPos)); */
        /* ceilVals = ceil(getIndexCordFromTerrain(curPos)); */
        /* if((floorVals.x < 0) || (floorVals.y < 0)){ curPos -= scale*tmpRay; return;} */
        /* if((ceilVals.x > mXDim - 1) || (ceilVals.y > mZDim - 1)){ curPos -= scale*tmpRay; return;} */

        indexCord0 = floor(getIndexCordFromTerrain(curPos));    // Bottom left
        indexCord3 = ceil(getIndexCordFromTerrain(curPos));     // Top right
        indexCord1 = glm::ivec2(indexCord0.x, indexCord3.y);    // Top left
        indexCord2 = glm::ivec2(indexCord3.x, indexCord0.y);    // Bottom right

        cord0 = getTerrainPosition(indexCord0); // Bottom left
        cord1 = getTerrainPosition(indexCord1); // Top left
        cord2 = getTerrainPosition(indexCord2); // Bottom right
        cord3 = getTerrainPosition(indexCord3); // Top right

        dist0 = sqrt((curPos.x - cord0.x)*(curPos.x - cord0.x) + (curPos.z - cord0.z)*(curPos.z - cord0.z));
        dist1 = sqrt((curPos.x - cord1.x)*(curPos.x - cord1.x) + (curPos.z - cord1.z)*(curPos.z - cord1.z));
        dist2 = sqrt((curPos.x - cord2.x)*(curPos.x - cord2.x) + (curPos.z - cord2.z)*(curPos.z - cord2.z));
        dist3 = sqrt((curPos.x - cord3.x)*(curPos.x - cord3.x) + (curPos.z - cord3.z)*(curPos.z - cord3.z));

        interpolHeight = dist0 * cord0.y + dist1 * cord1.y + dist2 * cord2.y + dist3 * cord3.y;
        /* rayHeight = camPosition.y + (tmpRay.y * distance); */
        /* std::cout << "index pos 0: x = " << indexCord0.x << ", y = " << indexCord0.y << std::endl; */
        /* std::cout << "index pos 1: x = " << indexCord1.x << ", y = " << indexCord1.y << std::endl; */
        /* std::cout << "index pos 2: x = " << indexCord2.x << ", y = " << indexCord2.y << std::endl; */
        /* std::cout << "index pos 3: x = " << indexCord3.x << ", y = " << indexCord3.y << std::endl; */
        /* std::cout << "current pos: x = " << curPos.x << ", y = " << curPos.y << ", z = " << curPos.z << "\t <=>" ; */
        /* std::cout << "terrain pos: x = " << cord0.x << ", y = " << cord0.y << ", z = " << cord0.z << std::endl; */


        /* if((glm::length(curPos.y - interpolHeight) < 0.1f) || (bisectState > 8) || (bisectState < 8) ) */
        /* { */
        /*     std::cout << "INTERSECTION at hight = " << interpolHeight << std::endl; */
        /*     break; */
        /* } */
        /* else if((bisectState > 0) && (curPos.y < interpolHeight)) */
        /* { */
        /*     scale *= -0.5; */
        /*     bisectState *= -2; */
        /*     std::cout << "bisectState = -1, scale = -0.5" << std::endl; */
        /* } */
        /* else if((bisectState < 0) && (curPos.y > interpolHeight)) */
        /* { */
        /*     scale *= -0.5; */
        /*     bisectState *= -2; */
        /*     std::cout << "bisectState = 1, scale = 0.5" << std::endl; */
        /* } */
        /* else if((curPos.y < interpolHeight) && (bisectState == 0)) */
        /* { */
        /*     bisectState = -1; */
        /*     scale = -0.5; */
        /*     std::cout << "starting bisection:" << std::endl; */
        /* } */

    }
    /* mRayTerrainIntersection = glm::vec3(curPos.x, curPos.y, -curPos.z); */

    updateArea(indexCord0, indexCord1, indexCord2, indexCord3);
    mRayTerrainIntersection = curPos;
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

    /* for(std::vector<glm::vec3>::iterator normal = mVertices.begin(); normal != mVertices.end(); normal += 2) */
    glm::vec3 c, l, r, t, b, tr, bl;
    glm::vec3 normalA, normalB, normalC, normalD, normalE, normalF, vNormal;
    unsigned int index;
    /* Triangle A, B, c, D, E, F; */
    for(unsigned int i = 0; i < mVertices.size(); i += mFloatsPerVertex)
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
        i1 = mIndices.at(i+1);
        i2 = mIndices.at(i+2);
        v0 = glm::vec3(mVertices.at(i0 * mFloatsPerVertex), mVertices.at(i0 * mFloatsPerVertex + 1), mVertices.at(i0 * mFloatsPerVertex + 2));
        v1 = glm::vec3(mVertices.at(i1 * mFloatsPerVertex), mVertices.at(i1 * mFloatsPerVertex + 1), mVertices.at(i1 * mFloatsPerVertex + 2));
        v2 = glm::vec3(mVertices.at(i2 * mFloatsPerVertex), mVertices.at(i2 * mFloatsPerVertex + 1), mVertices.at(i2 * mFloatsPerVertex + 2));

        deltaPos0 = glm::vec3(v1.x - v0.x, v1.y - v0.y, v1.z - v0.z);
        deltaPos1 = glm::vec3(v2.x - v0.x, v2.y - v0.y, v2.z - v0.z);

        tangent = (deltaPos0 * dUVz - deltaPos1 * dUVx) * r;
        bitangent = (deltaPos1 * dUVx - deltaPos0 * dUVx) * r;
        /* v1 = glm::vec3(mVertices.at(i), mVertices.at(i+1), mVertices(i+2)); */
        mVertices.at(i0 + 6) = tangent.x;
        mVertices.at(i0 + 7) = tangent.y;
        mVertices.at(i0 + 8) = tangent.z;
        mVertices.at(i1 + 6) = tangent.x;
        mVertices.at(i1 + 7) = tangent.y;
        mVertices.at(i1 + 8) = tangent.z;
        mVertices.at(i2 + 6) = tangent.x;
        mVertices.at(i2 + 7) = tangent.y;
        mVertices.at(i2 + 8) = tangent.z;

        mVertices.at(i0 + 9) = bitangent.x;
        mVertices.at(i0 + 10) = bitangent.y;
        mVertices.at(i0 + 11) = bitangent.z;
        mVertices.at(i1 + 9) = bitangent.x;
        mVertices.at(i1 + 10) = bitangent.y;
        mVertices.at(i1 + 11) = bitangent.z;
        mVertices.at(i2 + 9) = bitangent.x;
        mVertices.at(i2 + 10) = bitangent.y;
        mVertices.at(i2 + 11) = bitangent.z;

    }
}

void Terrain::buildVBO()
{
    // Position(3), Normal(3), Tangent(3), Bitangent(3), UV(2)
    mVertices.resize(mTotalVertices * mFloatsPerVertex);
    /* mVertices.resize(mTotalVertices * mElementsPerVertex); */

    computeTerrainPositions();

    if(mUseNormals)
    {
        computeTerrainNormals();
    }

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

void Terrain::build()
{
    buildVBO();
    buildIBO();
    /* buildDebug(); */
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

void Terrain::updateArea(glm::ivec2& bl, glm::ivec2& tl, glm::ivec2& br, glm::ivec2& tr )
{

    int blOffset = mFloatsPerVertex * ((bl.y * mXDim) + bl.x);
    int tlOffset = mFloatsPerVertex * ((tl.y * mXDim) + tl.x);
    int brOffset = mFloatsPerVertex * ((br.y * mXDim) + br.x);
    int trOffset = mFloatsPerVertex * ((tr.y * mXDim) + tr.x);
    /* std::cout << "buffer blOffset = " << blOffset << std::endl; */

    int relTlOffset = tlOffset - blOffset;
    int relBrOffset = brOffset - blOffset;
    int relTrOffset = trOffset - blOffset;

    // Set new hights
    mVertices.at(blOffset + 1) += 0.5f;
    mVertices.at(tlOffset + 1) += 0.5f;
    mVertices.at(brOffset + 1) += 0.5f;
    mVertices.at(trOffset + 1) += 0.5f;

    // Fill update buffer
    int size = relTrOffset + mFloatsPerVertex;
    float newData[size];
    /* std::cout << "buffer offset = " << blOffset << ", size = " << size << std::endl; */
    for(unsigned int i = 0; i < size; i++)
    {
        newData[i] = mVertices.at(blOffset + i);
    }


    /* newData[1] += 0.5f; */
    /* newData[relTlOffset + 1] += 0.5f; */
    /* newData[relBrOffset + 1] += 0.5f; */
    /* newData[relTrOffset + 1] += 0.5f; */


    /* int offset  = mFloatsPerVertex * ((coords.y * mXDim) + coords.x); */
    /* int size    = 60 * mFloatsPerVertex; */

    updateVertexBuffer(blOffset, size, newData);

}

void Terrain::updateVertexBuffer(int offset, int size, float* newVertexData)
{

    // Bind Attributes

    glBindBuffer(GL_ARRAY_BUFFER, mVbo);
    /* void *vertexData = glMapBufferRange(GL_ARRAY_BUFFER, 0, 3, GL_MAP_WRITE_BIT | GL_MAP_UNSYNCHRONIZED_BIT); */
    /* float* vertexBuffer = reinterpret_cast<float*>(glMapBufferRange(GL_ARRAY_BUFFER, offset*sizeof(GLfloat), size*sizeof(GLfloat), GL_MAP_WRITE_BIT)); */
    /* void* vertexBuffer = (glMapBufferRange(GL_ARRAY_BUFFER, offset*sizeof(GLfloat), size*sizeof(GLfloat), GL_MAP_WRITE_BIT)); */
    /* float* vertexData = reinterpret_cast<float*>(vertexBuffer); */
    float* vertexData = reinterpret_cast<float*>(glMapBufferRange(GL_ARRAY_BUFFER, offset*sizeof(GLfloat), size*sizeof(GLfloat), GL_MAP_WRITE_BIT));

    // Fill Buffer
    for(int i = 0; i < size; i++)
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
    muInvViewLocation = glGetUniformLocation(mShaderProgram, "uInvViewMatrix");
    muHeightMapTerrainRatioLocation = glGetUniformLocation(mShaderProgram, "uHeightMapTerrainRatio");
    muDrawGridLocation = glGetUniformLocation(mShaderProgram, "uDrawGrid");
    muRayTerrainIntersectionLocation = glGetUniformLocation(mShaderProgram, "uRayTerrainIntersection");
    muCamPos = glGetUniformLocation(mShaderProgram, "uCamPos");

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
    /* glVertexAttribPointer(terrainPosAttrib, 3, GL_FLOAT, GL_FALSE, mElementsPerVertex * sizeof(glm::vec3), 0); */
    if(mUseNormalMap)
    {
        // Normals
        glEnableVertexAttribArray(terrainNormalAttrib);
        glVertexAttribPointer(terrainNormalAttrib, 3, GL_FLOAT, GL_FALSE, mFloatsPerVertex * sizeof(GLfloat), reinterpret_cast<const GLvoid*>(3 * sizeof(GLfloat)));

        // Tangents
        glEnableVertexAttribArray(terrainTangentAttrib);
        glVertexAttribPointer(terrainNormalAttrib, 3, GL_FLOAT, GL_FALSE, mFloatsPerVertex * sizeof(GLfloat), reinterpret_cast<const GLvoid*>(6 * sizeof(GLfloat)));

        // Bitangents
        glEnableVertexAttribArray(terrainBitangentAttrib);
        glVertexAttribPointer(terrainNormalAttrib, 3, GL_FLOAT, GL_FALSE, mFloatsPerVertex * sizeof(GLfloat), reinterpret_cast<const GLvoid*>(9 * sizeof(GLfloat)));

        // UV
        glEnableVertexAttribArray(terrainUVAttrib);
        glVertexAttribPointer(terrainNormalAttrib, 2, GL_FLOAT, GL_FALSE, mFloatsPerVertex * sizeof(GLfloat), reinterpret_cast<const GLvoid*>(12 * sizeof(GLfloat)));

    }
    else if(mUseNormals)
    {
        glEnableVertexAttribArray(terrainNormalAttrib);
        /* glVertexAttribPointer(terrainNormalAttrib, 3, GL_FLOAT, GL_FALSE, 12, reinterpret_cast<const GLvoid*>(mFloatsPerVertex/2 * mTotalVertices * sizeof(GLfloat))); */
        glVertexAttribPointer(terrainNormalAttrib, 3, GL_FLOAT, GL_FALSE, mFloatsPerVertex * sizeof(GLfloat), reinterpret_cast<const GLvoid*>(3 * sizeof(GLfloat)));
        /* glVertexAttribPointer(terrainNormalAttrib, 3, GL_FLOAT, GL_FALSE, mElementsPerVertex * sizeof(glm::vec3), reinterpret_cast<const GLvoid*>(sizeof(glm::vec3))); */
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

