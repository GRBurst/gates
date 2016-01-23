#ifndef TERRAIN_H_
#define TERRAIN_H_
#include "Noise.h"
#include <GL/glew.h>
#include <iostream>
#include <stdio.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "NoiseType.h"
#include "Texture.h"

/*
 * Usage:
 * 1.) Construct Terrain();
 * 2.) If not set in 1.), setNoiseParams
 * 3.) Set vp matrix
 * 4.) Optional: genHeightMapTexture()
 * 5.) Optional: enableNormals()
 * 6.) computeTerrain()
 * 7.) draw()
 */
class Terrain {

    public:
        Terrain(GLint shaderProgram, int width, int height);
        Terrain(GLint shaderProgram, int width, int height, Noise *noise);
        ~Terrain();

        void generateHeights()
        {
        	mNoise->generateNoiseImage();
        	mNoiseValues = mNoise->getTextureData();
        };
        void computeTerrain()
        {
            generateHeights();
            build();
            setBuffers();
        };
        void genHeightMapTexture();
        void linkHeightMapTexture(GLint shader);

        void setInvViewMatrix(glm::mat3 iv) { this->mInvViewMatrix = iv; };
        void setVPMatrix(glm::mat4 vp) { this->mVPMatrix = vp; };

        void enableNormals() {mUseNormals = true; mFloatsPerVertex += 3;};
        void draw();

        float getWidth() const
        {
            return mWidth;
        };
        float getHeight() const
        {
            return mHeight;
        }
        glm::ivec2 getTerrainDimension()
        {
            return glm::ivec2(mWidth, mHeight);
        };
        glm::ivec2 getHeightMapDimension()
        {
            return glm::ivec2(mNoise->getDimension());
        };
        float getHeightMapValue(glm::ivec2 coordinate)
        {
            return mNoiseValues[(coordinate.y * (mNoise->getDimension()).x) + coordinate.x];
        };
        float getTerrainHeight(glm::ivec2 coordinate)
        {
            /* return mTerrainOffset + mTerrainScale * mNoiseValues[(coordinate.y * mWidth) + coordinate.x]; */
            return mVertices[3 * ((coordinate.y * mWidth) + coordinate.x) + 1];
        };
        glm::vec3 computePosition(int x, int y) const;

        float* getNoiseValues()
        {
            return mNoiseValues;
        };
        void setNoiseValues(float* noiseVal){mNoiseValues = noiseVal; };
        /* float* getHeightValues() */
        /* { */
        /*     return mNoiseValues; */
        /* }; */
        /* float* getTerrainValues() */
        /* { */
        /*     return mNoiseValues; */
        /* }; */

        void setGrid(int mode) {mDrawGrid = mode;};

        void saveNoiseToFile(const char* filename = "terrainNoise.tga")
        {
            mNoise->saveToFile(filename);
        }

        GLuint getVAO(){return mVao;}
        int getTotalIndices();

        void debug();

    private:
        int getSize();
        int getVerticeNumber();
        int getIndicesPerRow();
        int getDegensRequired();

        int getIndexLeftPosition(int index);
        int getIndexRightPosition(int index);
        int getIndexTopPosition(int index);
        int getIndexBottomPosition(int index);
        int getIndexTopRightPosition(int index);
        int getIndexBottomLeftPosition(int index);
        int computeTerrainPositions();
        //glm::vec3 computeNormals();
        void computeTerrainNormals(int &offset, int &length);

        void build();
        void setBuffers();
        void buildVBO();
        void buildIBO();
        void buildDebug();

        int mWidth, mHeight;
        int mTotalIndices, mTotalVertices;

        Noise *mNoise;
        float* mNoiseValues;
        NoiseType noiseType;

        Texture mHeightMapTexture;
        bool mUseHeightMapTexture;

        float mTerrainOffset;
        float mTerrainScale;
        float mTerrainMinPos;
        float mTerrainPosRange;
        bool mUseNormals;

        GLfloat* mVertices;
        GLint* mIndices;
        int mFloatsPerVertex;
        GLuint mVao;
        GLuint mVbo;
        GLuint mIbo;

        GLint mShaderProgram;
        GLuint muVPLocation;
        glm::mat4 mVPMatrix;
        GLuint muInvViewLocation;
        glm::mat3 mInvViewMatrix;
        GLint muHeightMapTerrainRatioLocation;
        int mHeightMapTerrainRatio;
        GLint muDrawGridLocation;
        int mDrawGrid;

        /* float minPos, posRange; */
        typedef struct
        {
            glm::vec3 a;
            glm::vec3 b;
            glm::vec3 c;
        } Triangle;
        // normal, counterclockwise edges
        glm::vec3 computeTriangleNormal(Triangle &t)
        {
            return glm::cross(glm::vec3(t.c - t.a), glm::vec3(t.b - t.a));
        };
        glm::vec3 computeVertexNormal(glm::vec3 v1, glm::vec3 v2, glm::vec3 v3, glm::vec3 v4, glm::vec3 v5, glm::vec3 v6 )
        {
            return glm::normalize(v1 + v2 + v3 + v4 + v5 + v6);
        };

};
#endif /* TERRAIN_H_ */
