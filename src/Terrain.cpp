#include "Terrain.h"
#include <iostream>

Terrain::Terrain(GLint shaderProgram, unsigned int xDim, unsigned int zDim, Camera *const camera, Noise *const noise)
    : mXDim(xDim)
    , mZDim(zDim)
    , mTotalVertices(xDim * zDim)
    , mHeightMapTexture()
{
    this->mShaderProgram = shaderProgram;
    this->mCamera = camera;
    this->mNoise = noise;

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
        //glDrawElements(GL_TRIANGLE_STRIP, mTotalIndices, GL_UNSIGNED_INT, 0);
        /* glDrawElements(GL_TRIANGLES, mTotalIndices, GL_UNSIGNED_INT, 0); */
        glDrawElements(GL_TRIANGLES, mTotalIndices, GL_UNSIGNED_INT, 0);
    }
    else if(mDrawGrid == 1)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glUniform1i(muDrawGridLocation, mDrawGrid);
        glDrawElements(GL_TRIANGLES, mTotalIndices, GL_UNSIGNED_INT, 0);
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
}

glm::vec3 Terrain::getTerrainPosition(const glm::ivec2& coordinate) const
{
    unsigned int index = mFloatsPerVertex * ((coordinate.y * mXDim) + coordinate.x);
    return glm::vec3(mVertices.at(index), mVertices.at(index+1), mVertices.at(index+2));
}

glm::vec2 Terrain::getIndexCordFromTerrain(const glm::vec3& position) const
{
    float x = (mXDim - 1) * ((position.x - mTerrainMinPos) / mTerrainPosRange);
    float z = (mZDim - 1) * (1 - ((position.z - mTerrainMinPos) / mTerrainPosRange));
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

    float interpolHeight;
    float dist0, dist1, dist2, dist3;
    glm::vec3 cord0, cord1, cord2, cord3;
    glm::ivec2 floorVals, ceilVals;
    glm::ivec2 indexCord0, indexCord1, indexCord2, indexCord3;

    glm::vec3 curPos = mCamera->getPosition();
    glm::vec3 tmpRay = ray;
    while( (curPos.y > -0.5f) && (curPos.y < 25.0f))
    {
        curPos += tmpRay;

        floorVals = floor(getIndexCordFromTerrain(curPos));
        ceilVals = ceil(getIndexCordFromTerrain(curPos));

        indexCord0 = glm::ivec2(floorVals.x, floorVals.y);   // Bottom left
        indexCord1 = glm::ivec2(floorVals.x, ceilVals.y);    // Top left
        indexCord2 = glm::ivec2(ceilVals.x, floorVals.y);    // Bottom right
        indexCord3 = glm::ivec2(ceilVals.x, ceilVals.y);     // Top right

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

        if(curPos.y < interpolHeight) break;

    }
    mRayTerrainIntersection = curPos;
}

// Noise getters
void Terrain::saveNoiseToFile(const char* filename) const
{
    mNoise->saveToFile(filename);
}

void Terrain::debug()
{

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
    for(unsigned int i = 0; i < mTotalVertices; i += mFloatsPerVertex)
    {
        printf("vertice:\t x = %5.2f, y = %5.2f, z = %5.2f\n" , mVertices.at(i+0), mVertices.at(i+1), mVertices.at(i+2) );
        printf("normal:\t\t x = %5.2f, y = %5.2f, z = %5.2f\n" , mVertices.at(i+3), mVertices.at(i+4), mVertices.at(i+5) );
    }

    std::cout << "noise values(" << mNoiseValues.size() << "):" << std::endl;
    for(unsigned int i = 0; i < mTotalVertices; i++)
    {
        printf("noise value = %5.2f\n" , mNoiseValues.at(i));
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
}

unsigned int Terrain::getIndexRightPosition(unsigned int index) const
{
    /* return (index + 3) % (3*mTotalVertices); */
    return (index + mFloatsPerVertex) % (mFloatsPerVertex * mTotalVertices);
}

unsigned int Terrain::getIndexTopPosition(unsigned int index) const
{
    /* return (index - (3 * mXDim)) % (3*mTotalVertices); */
    return ((mFloatsPerVertex * mTotalVertices) + (index - (mFloatsPerVertex * mXDim))) % (mFloatsPerVertex * mTotalVertices);
}

unsigned int Terrain::getIndexBottomPosition(unsigned int index) const
{
    /* return (index + (3 * mXDim)) % (3*mTotalVertices); */
    return (index + (mFloatsPerVertex * mXDim)) % (mFloatsPerVertex * mTotalVertices);
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
    }
}

void Terrain::buildVBO()
{
    // Position(3), Normal(3), Tangent(3), Bitangent(3)
    mVertices.resize(mTotalVertices * mFloatsPerVertex);

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



/*     int offset = 0; */
/*     for(unsigned int z = 0; z < mZDim - 1; z++) */
/*     { */
/*         for(unsigned int x = 0; x < mXDim - 1; x++) */
/*         { */
/*             mIndices[offset++] = ((z * mXDim) + x); */
/*             mIndices[offset++] = (((z+1) * mXDim) + x); */
/*             mIndices[offset++] = ((z * mXDim) + x + 1); */

/*             mIndices[offset++] = ((z * mXDim) + x + 1); */
/*             mIndices[offset++] = (((z+1) * mXDim) + x); */
/*             mIndices[offset++] = (((z+1) * mXDim) + x + 1); */
/*         } */

/*     } */

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

void Terrain::setBuffers()
{
    //Unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glUseProgram(mShaderProgram);

    // Get Attrib location
    GLint terrainPosAttrib = glGetAttribLocation(mShaderProgram, "vPosition");
    GLint terrainNormalAttrib = glGetAttribLocation(mShaderProgram, "vNormal");
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
    glBindBuffer(GL_ARRAY_BUFFER, mVbo);
    glBufferData(GL_ARRAY_BUFFER, mVertices.size() * sizeof(GLfloat), mVertices.data(), GL_STATIC_DRAW);

    // First all positions, then all normals
    glEnableVertexAttribArray(terrainPosAttrib);
    glVertexAttribPointer(terrainPosAttrib, 3, GL_FLOAT, GL_FALSE, mFloatsPerVertex * sizeof(GLfloat), 0);
    if(mUseNormals)
    {
        glEnableVertexAttribArray(terrainNormalAttrib);
        /* glVertexAttribPointer(terrainNormalAttrib, 3, GL_FLOAT, GL_FALSE, 12, reinterpret_cast<const GLvoid*>(mFloatsPerVertex/2 * mTotalVertices * sizeof(GLfloat))); */
        glVertexAttribPointer(terrainNormalAttrib, 3, GL_FLOAT, GL_FALSE, mFloatsPerVertex * sizeof(GLfloat), reinterpret_cast<const GLvoid*>(mFloatsPerVertex / mElementsPerVertex * sizeof(GLfloat)));
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

