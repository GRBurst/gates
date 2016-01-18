#include "Terrain.h"
#include <iostream>

Terrain::Terrain(GLint shaderProgram, int width, int height)
    : mWidth(width)
    , mHeight(height)
{
	this->mShaderProgram = shaderProgram;
    mTotalVertices = width * height;
    glm::vec2 terrainSize(static_cast<float>(width), static_cast<float>(height));

    int numStripsRequired = mHeight - 1;
    int numDegensRequired = 2 * (numStripsRequired - 1);
    int verticesPerStrip = 2 * mWidth;
    mTotalIndices = (verticesPerStrip * numStripsRequired) + numDegensRequired;
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

void Terrain::buildVBO(int floatsPerVertex)
{
    float minPos = -5.0f;
    float posRange = 10.0f;

    mVertices = new GLfloat[mTotalVertices * floatsPerVertex];

    int offset = 0;
    for(int y = 0; y < mHeight; y++)
    {
        for(int x = 0; x < mWidth; x++)
        {
            float xRatio = static_cast<float>(x) / static_cast<float>(mWidth -1);
            float yRatio = 1.0f - (static_cast<float>(y) / static_cast<float>(mHeight - 1));

            float xPosition = minPos + (xRatio * posRange);
            float yPosition = minPos + (yRatio * posRange);

            mVertices[offset++] = xPosition;
            mVertices[offset++] = yPosition;
            /* std::cout << "xPos = " << xPosition << std::endl; */
            /* std::cout << "yPos = " << yPosition << std::endl; */

        }
    }

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

}

void Terrain::build()
{
    buildVBO();
    buildIBO();
}

void Terrain::setup()
{
    //Unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glUseProgram(mShaderProgram);

    GLint terrainPosAttrib = glGetAttribLocation(mShaderProgram, "vPosition");
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
    glEnableVertexAttribArray(terrainPosAttrib);
    glVertexAttribPointer(terrainPosAttrib, 2, GL_FLOAT, GL_FALSE, 0, 0);

    //Generate & bind ibo
    glGenBuffers(1, &mIbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIbo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mTotalIndices * sizeof(GLint), mIndices, GL_STATIC_DRAW);

    //Unbind
    glBindVertexArray(0);
    delete mVertices;
    delete mIndices;
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
