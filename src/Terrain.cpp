#include "Terrain.h"
#include <iostream>

Terrain::Terrain(GLint shaderProgram, int width, int height)
    : mWidth(width)
    , mHeight(height)
{
	this->mShaderProgram = shaderProgram;
    mTotalVertices = width * height;
    glm::vec2 terrainSize(static_cast<float>(width), static_cast<float>(height));

    mTerrainMinPos = -5.0f;
    mTerrainPosRange = 10.0f;
    mHeightMapTerrainRatio = 1;
    mFloatsPerVertex = 3;

    int numStripsRequired = mHeight - 1;
    int numDegensRequired = 2 * (numStripsRequired - 1);
    int verticesPerStrip = 2 * mWidth;
    mTotalIndices = (verticesPerStrip * numStripsRequired) + numDegensRequired;
    Texture mHeightMapTexture();
}

Terrain::Terrain(GLint shaderProgram, int width, int height, int x, int y, Noise::NoiseType noiseType, int seed, int octaves, double frequency, double amplitude) 
    : Terrain(shaderProgram, width, height)
{
    mNoise.setParams(x, y, noiseType, seed, octaves, frequency, amplitude);
}

Terrain::~Terrain()
{
}

/* void Terrain::setHeigthMapData(float* heights) */
/* { */
/*     mHeigtData = heights; */
/* } */

int Terrain::getSize()
{
    return mWidth * mHeight;
}

int Terrain::getVerticeNumber()
{
    return mTotalVertices;
}

int Terrain::getTotalIndices()
{
    return mTotalIndices;
}

int Terrain::getIndicesPerRow()
{
    return 2 * mWidth + 2;
}

int Terrain::getDegensRequired()
{
    return (mWidth - 1) * 2;
}

void Terrain::setVPMatrix(glm::mat4 vp)
{
    this->mVPMatrix = vp;
}

glm::vec3 Terrain::computePosition(int x, int y)
{

    float xRatio = static_cast<float>(x) / static_cast<float>(mWidth -1);
    float yRatio = 1.0f - (static_cast<float>(y) / static_cast<float>(mHeight - 1));

    float xPosition = mTerrainMinPos + (xRatio * mTerrainPosRange);
    float yPosition = mTerrainMinPos + (yRatio * mTerrainPosRange);
    float zPosition = mNoiseValues[y * mHeightMapTerrainRatio * mNoise.getWidth() + x];
    /* double zPosition = mNoise.calculateNoiseValue((double)x * (double)mHeightMapTerrainRatio, (double)y * (double)mHeightMapTerrainRatio); */
    /* std::cout << "zPosition = " << zPosition << std::endl; */

    return glm::vec3(xPosition, yPosition, zPosition);
}

int Terrain::computeTerrainPositions()
{
    glm::vec3 position;
    int offset = 0;
    for(int y = 0; y < mHeight; y++)
    {
        for(int x = 0; x < mWidth; x++)
        {

            // Position calculation
            position = computePosition(x, y);

            mVertices[offset++] = position.x;
            mVertices[offset++] = position.y;
            mVertices[offset++] = position.z;

        }
    }

    return offset;
}

void Terrain::computeTerrainNormals(int &offset, int &length)
{

    int j;
    for(int i = offset; i < length; i += 3)
    {
        glm::vec3 c(mVertices[i], mVertices[i+1], mVertices[i+2]);
        // Get surrounding vertices
        j = getIndexLeftPosition(i);
        glm::vec3 l(mVertices[j], mVertices[j+1], mVertices[j+2]);
        j = getIndexRightPosition(i);
        glm::vec3 r(mVertices[j], mVertices[j+1], mVertices[j+2]);
        j = getIndexTopPosition(i);
        glm::vec3 t(mVertices[j], mVertices[j+1], mVertices[j+2]);
        j = getIndexBottomPosition(i);
        glm::vec3 b(mVertices[j], mVertices[j+1], mVertices[j+2]);
        j = getIndexTopRightPosition(i);
        glm::vec3 tr(mVertices[j], mVertices[j+1], mVertices[j+2]);
        j = getIndexBottomLeftPosition(i);
        glm::vec3 bl(mVertices[j], mVertices[j+1], mVertices[j+2]);

        // Triangle
        Triangle A = {c, l, t};
        Triangle B = {c, t, tr};
        Triangle C = {c, tr,r};
        Triangle D = {c, r, b};
        Triangle E = {c, b, bl};
        Triangle F = {c, bl,l};

        glm::vec3 normalA = computeTriangleNormal(A);
        glm::vec3 normalB = computeTriangleNormal(B);
        glm::vec3 normalC = computeTriangleNormal(C);
        glm::vec3 normalD = computeTriangleNormal(D);
        glm::vec3 normalE = computeTriangleNormal(E);
        glm::vec3 normalF = computeTriangleNormal(F);

        glm::vec3 vNormal = computeVertexNormal(normalA, normalB, normalC, normalD, normalE, normalF);

        mVertices[i+0] = vNormal.x;
        mVertices[i+1] = vNormal.y;
        mVertices[i+2] = vNormal.z;

        //std::cout << "normal =" << vNormal.x << ", " << vNormal.y << ", " << vNormal.z << std::endl;
    }

}

int Terrain::getIndexLeftPosition(int index)
{
    /* return (mTotalVertices + index - 3) % mTotalVertices; */
    /* int ni = index - 3 -(3*mTotalVertices); */
    /* if((ni < bound) || (ni < 0)) ni += 3*mTotalVertices; */
    /* return ni; */
    return (index - 3) % (3*mTotalVertices);
}

int Terrain::getIndexRightPosition(int index)
{
    /* return (mTotalVertices + index + 3) % mTotalVertices; */
    /* int ni = index + 3; */
    /* if((ni > bound) || (ni > 6*mTotalVertices)) ni -= 3*mTotalVertices; */
    /* return ni; */
    return (index + 3) % (3*mTotalVertices);
}

int Terrain::getIndexTopPosition(int index)
{
    /* return (mTotalVertices + index - (3 * mWidth)) % mTotalVertices; */
    /* int ni = index - (3 * mWidth); */
    /* if((ni < bound) || (ni < 0)) ni += 3*mTotalVertices; */
    /* return ni; */
    return (index - (3 * mWidth)) % (3*mTotalVertices);
}

int Terrain::getIndexBottomPosition(int index)
{
    /* return (mTotalVertices + index + (3 * mWidth)) % mTotalVertices; */
    /* int ni = index + (3 * mWidth); */
    /* if((ni > bound) || (ni > 6*mTotalVertices)) ni -= 3*mTotalVertices; */
    /* return ni; */
    return (index + 3) % (3*mTotalVertices);
}

int Terrain::getIndexTopRightPosition(int index)
{
    return getIndexRightPosition(getIndexTopPosition(index));
}

int Terrain::getIndexBottomLeftPosition(int index)
{
    return getIndexLeftPosition(getIndexBottomPosition(index));
}

void Terrain::buildVBO()
{
    // Position(3), Normal(3), Tangent(3), Bitangent(3)
    // mFloatsPerVertex = 6;

    int length = mTotalVertices * mFloatsPerVertex;
    mVertices = new GLfloat[length];

    int offset = computeTerrainPositions();

    /* std::cout << std::endl << "offset = " << offset << std::endl; */
    /* std::cout << std::endl << "w*h = " << mWidth*mHeight << std::endl; */
    /* std::cout << std::endl << "numVert = " << mTotalVertices << std::endl; */
    /* std::cout << std::endl << "length = " << length << std::endl; */

    if(mUseNormals)
    {
        computeTerrainNormals(offset, length);
    }


    /* std::cout << std::endl << std::endl; */
    /* std::cout << mVertices[0]; */
    /* for(int i = 1; i < offset; i++) */
    /* { */
    /*     std::cout << ", " << mVertices[i]; */
    /* } */
    /* std::cout << std::endl << std::endl; */
}

void Terrain::buildIBO()
{

    mIndices = new GLint[mTotalIndices];

    int offset = 0;
    for(int y = 0; y < mHeight - 1; y++)
    {
        if(y > 0)
            mIndices[offset++] = (y * mWidth);

        for(int x = 0; x < mWidth; x++)
        {
            mIndices[offset++] = ((y * mWidth) + x);
            mIndices[offset++] = (((y+1) * mWidth) + x);
        }

        if(y < mHeight - 2)
            mIndices[offset++] = (((y+1) * mWidth) + (mWidth - 1));

    }

    /* std::cout << std::endl << std::endl; */
    /* std::cout << mIndices[0]; */
    /* for(int i = 1; i < offset; i++) */
    /* { */
    /*     std::cout << ", " << mIndices[i]; */
    /* } */
    /* std::cout << std::endl << std::endl; */

}

void Terrain::genHeightMapTexture()
{
    mUseHeightMapTexture = true;
    mHeightMapTexture.bind();
    mHeightMapTexture.setData(mNoise.getTextureDataF(), mNoise.getWidth(), mNoise.getHeight());
    mHeightMapTexture.loadHeightmapOptions();
    mHeightMapTexture.linkTexture(mShaderProgram, "heightMap");
}

void Terrain::linkHeightMapTexture(GLint shader)
{
    mHeightMapTexture.linkTexture(shader, "heightMap");
}

void Terrain::build()
{
    buildVBO();
    buildIBO();
}

void Terrain::setBuffers()
{
    //Unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glUseProgram(mShaderProgram);

    GLint terrainPosAttrib = glGetAttribLocation(mShaderProgram, "vPosition");
    GLint terrainNormalAttrib = glGetAttribLocation(mShaderProgram, "vNormal");
    mVPLocation = glGetUniformLocation(mShaderProgram, "VPMatrix");
    mTerrainSizeLocation = glGetUniformLocation(mShaderProgram, "terrainSize");

    //Generate & bind vao
    glGenVertexArrays(1, &mVao);
    glBindVertexArray(mVao);

    //Generate & bind vbo
    glGenBuffers(1, &mVbo);
    glBindBuffer(GL_ARRAY_BUFFER, mVbo);
    glBufferData(GL_ARRAY_BUFFER, getVerticeNumber() * sizeof(GLfloat), mVertices, GL_STATIC_DRAW);

    // Get Attrib location
    /* glEnableVertexAttribArray(terrainPosAttrib); */
    /* glVertexAttribPointer(terrainPosAttrib, 3, GL_FLOAT, GL_FALSE, 24, 0); */
    /* glEnableVertexAttribArray(terrainNormalAttrib); */
    /* glVertexAttribPointer(terrainNormalAttrib, 3, GL_FLOAT, GL_FALSE, 24, reinterpret_cast<const GLvoid*>(12)); */

    // First all positions, then all normals
    glEnableVertexAttribArray(terrainPosAttrib);
    glVertexAttribPointer(terrainPosAttrib, 3, GL_FLOAT, GL_FALSE, 12, 0);
    if(mUseNormals)
    {
        glEnableVertexAttribArray(terrainNormalAttrib);
        glVertexAttribPointer(terrainNormalAttrib, 3, GL_FLOAT, GL_FALSE, 12, reinterpret_cast<const GLvoid*>(4*3*mTotalVertices));
    }

    //Generate & bind ibo
    glGenBuffers(1, &mIbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIbo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mTotalIndices * sizeof(GLint), mIndices, GL_STATIC_DRAW);

    //Unbind
    glBindVertexArray(0);
    /* delete mVertices; */
    /* delete mIndices; */
    //glBindBuffer(GL_ARRAY_BUFFER, 0);
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

void Terrain::draw()
{
    glUseProgram(mShaderProgram);
    glUniformMatrix4fv(mVPLocation, 1, GL_FALSE, value_ptr(mVPMatrix));
    glUniformMatrix4fv(mTerrainSizeLocation, 1, GL_FALSE, value_ptr(mTerrainSize));

    // Bind Attributes
    glBindVertexArray(mVao);

    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIbo);
    /* int size; */
    /* glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size); */
    /* std::cout << "buffer size = " << size << ", vertice numeber = " << getVerticeNumber() << ", indice number = " << totalIndices << std::endl; // size/sizeof(GLuint)*/
    glDrawElements(GL_TRIANGLE_STRIP, mTotalIndices, GL_UNSIGNED_INT, 0);

    //glBindBuffer(GL_ARRAY_BUFFER, 0);
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Terrain::debug()
{
    std::cout << std::endl << std::endl;
    std::cout << "vertices:" << std::endl;
    for(int i = 0; i < 3*mTotalVertices; i++)
    {
        if((i % 3) == 0) std::cout << std::endl << mVertices[i];
        else std::cout << ", " << mVertices[i];
    }
    std::cout << std::endl << std::endl;

    std::cout << std::endl << std::endl;
    std::cout << "indices:" << std::endl;
    std::cout << mIndices[0];
    for(int i = 1; i < mTotalIndices; i++)
    {
        std::cout << ", " << mIndices[i];
    }
    std::cout << std::endl << std::endl;

    std::cout << "normals:" << std::endl;
    for(int i = 3*mTotalVertices; i < 6*mTotalVertices; i++)
    {
        if((i % 3) == 0) std::cout << std::endl << mVertices[i];
        else std::cout << ", " << mVertices[i];
    }
    std::cout << std::endl << std::endl;
}
