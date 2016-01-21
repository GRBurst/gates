#include "Terrain.h"
#include <iostream>

Terrain::Terrain(GLint shaderProgram, int width, int height)
    : mWidth(width)
    , mHeight(height)
{
	this->mShaderProgram = shaderProgram;
    mTotalVertices = width * height;
    glm::vec2 terrainSize(static_cast<float>(width), static_cast<float>(height));

    /* mTerrainPosRange = static_cast<float>(width); */
    /* mTerrainMinPos = -static_cast<float>(width/2); */
    mTerrainPosRange = 50.0f;
    mTerrainMinPos = -25.0f;
    mHeightMapTerrainRatio = 1;
    mFloatsPerVertex = 3;
    mDrawGrid = 0;

    int numStripsRequired = mHeight - 1;
    int numDegensRequired = 2 * (numStripsRequired - 1);
    int verticesPerStrip = 2 * mWidth;
    mTotalIndices = (verticesPerStrip * numStripsRequired) + numDegensRequired;
    Texture mHeightMapTexture();
}

Terrain::Terrain(GLint shaderProgram, int width, int height, PerlinNoise &noise)
    : Terrain(shaderProgram, width, height)
{
	this->mNoise = noise;
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

glm::vec3 Terrain::computePosition(int x, int z)
{

    float xRatio = static_cast<float>(x) / static_cast<float>(mWidth -1);
    float zRatio = 1.0f - (static_cast<float>(z) / static_cast<float>(mHeight - 1));

    float xPosition = mTerrainMinPos + (xRatio * mTerrainPosRange);
    float yPosition = mNoiseValues[z * mHeightMapTerrainRatio * mNoise.getWidth() + x];
    float zPosition = mTerrainMinPos + (zRatio * mTerrainPosRange);
    /* double zPosition = mNoise.calculateNoiseValue((double)x * (double)muHeightMapTerrainRatio, (double)y * (double)muHeightMapTerrainRatio); */
    /* std::cout << "zPosition = " << zPosition << std::endl; */

    return glm::vec3(xPosition, yPosition, -zPosition);
    /* return glm::vec3(x, y, -1.0); */
}

int Terrain::computeTerrainPositions()
{
    glm::vec3 position;
    int offset = 0;
    for(int z = 0; z < mHeight; z++)
    {
        for(int x = 0; x < mWidth; x++)
        {

            // Position calculation
            position = computePosition(x, z);

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

        // Triangle, clockwise vertices
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

        /* mVertices[i+0] = 1.0; */
        /* mVertices[i+1] = 0.0; */
        /* mVertices[i+2] = 0.0; */
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

    mTotalIndices = (6 * (mWidth-1) * (mHeight-1));
    /* std::cout << "mTotalIndices = " << mTotalIndices << ", mTotalVertices = " << mTotalVertices << std::endl; */
    mIndices = new GLint[mTotalIndices];

    int offset = 0;
    for(int z = 0; z < mHeight - 1; z++)
    {
        for(int x = 0; x < mWidth - 1; x++)
        {
            mIndices[offset++] = ((z * mWidth) + x);
            mIndices[offset++] = (((z+1) * mWidth) + x);
            mIndices[offset++] = ((z * mWidth) + x + 1);

            mIndices[offset++] = ((z * mWidth) + x + 1);
            mIndices[offset++] = (((z+1) * mWidth) + x);
            mIndices[offset++] = (((z+1) * mWidth) + x + 1);
        }

    }

    /* for(int z = 0; z < mHeight - 1; z++) */
    /* { */
    /*     if(z > 0) */
    /*         mIndices[offset++] = (z * mWidth); */

    /*     for(int x = 0; x < mWidth; x++) */
    /*     { */
    /*         mIndices[offset++] = ((z * mWidth) + x); */
    /*         mIndices[offset++] = (((z+1) * mWidth) + x); */
    /*     } */

    /*     if(z < mHeight - 2) */
    /*         mIndices[offset++] = (((z+1) * mWidth) + (mWidth - 1)); */

    /* } */

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
    mHeightMapTexture.setData(mNoise.getTextureData(), mNoise.getWidth(), mNoise.getHeight());
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
    /* buildDebug(); */
}

void Terrain::buildDebug()
{

    mTotalVertices = 9;
    mTotalIndices = 3;

    mVertices = new GLfloat[mTotalVertices]{0, 0, 0, 0, 1, 0, 1, 1, 0};
    mIndices = new GLint[mTotalVertices];
    //Generate & bind ibo
    mIndices[0] = 0;
    mIndices[1] = 1;
    mIndices[2] = 2;

}

void Terrain::setBuffers()
{
    //Unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glUseProgram(mShaderProgram);

    GLint terrainPosAttrib = glGetAttribLocation(mShaderProgram, "vPosition");
    GLint terrainNormalAttrib = glGetAttribLocation(mShaderProgram, "vNormal");
    muVPLocation = glGetUniformLocation(mShaderProgram, "uVPMatrix");
    muInvViewLocation = glGetUniformLocation(mShaderProgram, "uInvViewMatrix");
    muHeightMapTerrainRatioLocation = glGetUniformLocation(mShaderProgram, "uHeightMapTerrainRatio");
    muDrawGridLocation = glGetUniformLocation(mShaderProgram, "uDrawGrid");

    //Generate & bind vao
    glGenVertexArrays(1, &mVao);
    glBindVertexArray(mVao);

    //Generate & bind vbo
    glGenBuffers(1, &mVbo);
    glBindBuffer(GL_ARRAY_BUFFER, mVbo);
    glBufferData(GL_ARRAY_BUFFER, mFloatsPerVertex*mTotalVertices * sizeof(GLfloat), mVertices, GL_STATIC_DRAW);

    // Get Attrib location
    /* glEnableVertexAttribArray(terrainPosAttrib); */
    /* glVertexAttribPointer(terrainPosAttrib, 3, GL_FLOAT, GL_FALSE, 24, 0); */
    /* glEnableVertexAttribArray(terrainNormalAttrib); */
    /* glVertexAttribPointer(terrainNormalAttrib, 3, GL_FLOAT, GL_FALSE, 24, reinterpret_cast<const GLvoid*>(12)); */

    // First all positions, then all normals
    glEnableVertexAttribArray(terrainPosAttrib);
    glVertexAttribPointer(terrainPosAttrib, 3, GL_FLOAT, GL_FALSE, 12, 0);
    /* glVertexAttribPointer(terrainPosAttrib, 3, GL_FLOAT, GL_FALSE, 0, 0); */
    if(mUseNormals)
    {
        glEnableVertexAttribArray(terrainNormalAttrib);
        glVertexAttribPointer(terrainNormalAttrib, 3, GL_FLOAT, GL_FALSE, 12, reinterpret_cast<const GLvoid*>(mFloatsPerVertex/2 * mTotalVertices * sizeof(GLfloat)));
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
    glUniformMatrix4fv(muVPLocation, 1, GL_FALSE, value_ptr(mVPMatrix));
    glUniformMatrix3fv(muInvViewLocation, 1, GL_FALSE, value_ptr(mInvViewMatrix));
    glUniform1i(muHeightMapTerrainRatioLocation, mHeightMapTerrainRatio);
    glUniform1i(muDrawGridLocation, mDrawGrid);

    // Bind Attributes
    glBindVertexArray(mVao);

    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIbo);
    /* int size; */
    /* glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size); */
    /* std::cout << "buffer size = " << size << ", vertice numeber = " << getVerticeNumber() << ", indice number = " << totalIndices << std::endl; // size/sizeof(GLuint)*/

    if(mDrawGrid == 0)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glUniform1i(muDrawGridLocation, mDrawGrid);
        //glDrawElements(GL_TRIANGLE_STRIP, mTotalIndices, GL_UNSIGNED_INT, 0);
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

    //glBindBuffer(GL_ARRAY_BUFFER, 0);
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Terrain::debug()
{

    /* std::cout << std::endl << std::endl; */
    /* std::cout << "Normals(:" << mTotalVertices << "): " << std::endl; */
    /* for(int i = 0; i < 3*mTotalVertices; i+= 3) */
    /* { */
    /*     printf("vertice:\t x = %5.2f, y = %5.2f, z = %5.2f\n" , mVertices[i], mVertices[i+1], mVertices[i+2]); */
    /*     printf("normal:\t\t x = %5.2f, y = %5.2f, z = %5.2f\n" , mVertices[3*mTotalVertices+i+1], mVertices[3*mTotalVertices+i+2], mVertices[3*mTotalVertices+i+3]); */
    /* } */
    /* std::cout << std::endl << std::endl; */

/*     std::cout << std::endl << std::endl; */
/*     std::cout << "vertices(" << mTotalVertices << "):" << std::endl; */
/*     for(int i = 0; i < 3*mTotalVertices; i++) */
/*     { */
/*         if((i % 3) == 0) std::cout << std::endl << mVertices[i]; */
/*         else std::cout << ", " << mVertices[i]; */
/*     } */
/*     std::cout << std::endl << std::endl; */
    
    std::cout << "vertices(" << mTotalVertices << "):" << std::endl;
    for(int i = 0; i < mTotalVertices; i++)
    {
        if((i % mWidth) == 0) std::cout << std::endl << i;
        else std::cout << "\t" << i;
    }
    std::cout << std::endl << std::endl;

    std::cout << "indices(" << mTotalIndices << "):" << std::endl;
    for(int i = 0; i < mTotalIndices; i++)
    {
        if((i % 6) == 0) std::cout << std::endl << mIndices[i];
        else std::cout << ", " << mIndices[i];
    }
    std::cout << std::endl << std::endl;






    /* std::cout << std::endl << std::endl; */
    /* std::cout << "heights:" << std::endl; */
    /* for(int i = 2; i < 3*mTotalVertices; i+= 3) */
    /* { */
    /*     if(((i-2)% mWidth) == 0) printf("\n%5.2f" , mVertices[i]); */
    /*     else printf(", %5.2f" , mVertices[i]); */
    /* } */
    /* std::cout << std::endl << std::endl; */

/*     std::cout << std::endl << std::endl; */
/*     std::cout << "STRANGE heights:" << std::endl; */
/*     for(int i = 2; i < 3*mTotalVertices; i+= 3) */
/*     { */
/*         if(((mVertices[i] <= 0.0) || (mVertices[i] > 2.4)) && (mVertices[3*mTotalVertices+i-2] == 0.0) && (mVertices[3*mTotalVertices+i-1] == 0.0) && (mVertices[3*mTotalVertices+i] == 0.0)) */
/*         { */
/*             printf("vertice:\t x = %5.2f, y = %5.2f, z = %5.2f\n" , mVertices[i-2], mVertices[i-1], mVertices[i]); */
/*             printf("normal:\t\t x = %5.2f, y = %5.2f, z = %5.2f\n" , mVertices[3*mTotalVertices+i+1], mVertices[3*mTotalVertices+i+2], mVertices[3*mTotalVertices+i+3]); */
/*         } */

/*     } */
/*     std::cout << std::endl << std::endl; */


/*     std::cout << "Origin:" << std::endl; */
/*     printf("vertex: x = %5.2f, y = %5.2f, z = %5.2f\n" , mVertices[0], mVertices[1], mVertices[2]); */
/*     printf("normal: x = %5.2f, y = %5.2f, z = %5.2f\n" , mVertices[3*mTotalVertices], mVertices[3*mTotalVertices+1], mVertices[3*mTotalVertices+2]); */

/*     std::cout << std::endl << std::endl; */
/*     std::cout << "vertices:" << std::endl; */
/*     for(int i = 0; i < 3*mTotalVertices; i++) */
/*     { */
/*         if((i % 3) == 0) std::cout << std::endl << mVertices[i]; */
/*         else std::cout << ", " << mVertices[i]; */
/*     } */
/*     std::cout << std::endl << std::endl; */

/*     std::cout << std::endl << std::endl; */
/*     std::cout << "indices:" << std::endl; */
/*     std::cout << mIndices[0]; */
/*     for(int i = 1; i < mTotalIndices; i++) */
/*     { */
/*         std::cout << ", " << mIndices[i]; */
/*     } */
/*     std::cout << std::endl << std::endl; */

/*     std::cout << "normals:" << std::endl; */
/*     for(int i = 3*mTotalVertices; i < 6*mTotalVertices; i++) */
/*     { */
/*         if((i % 3) == 0) std::cout << std::endl << mVertices[i]; */
/*         else std::cout << ", " << mVertices[i]; */
/*     } */
/*     std::cout << std::endl << std::endl; */
}
