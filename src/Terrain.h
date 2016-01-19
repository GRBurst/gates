#ifndef TERRAIN_H_
#define TERRAIN_H_

#include <GL/glew.h>
#include <iostream>
#include <stdio.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Noise.h"
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
        Terrain(GLint shaderProgram, int width, int height, int x, int y, Noise::NoiseType noiseType, int seed, int octaves, double frequency, double amplitude);
        ~Terrain();

        void setNoiseParams(int x, int y, Noise::NoiseType noiseType, int seed, int octaves, double frequency, double amplitude)
        {
            mNoise.setParams(x, y, noiseType, seed, octaves, frequency, amplitude);
        };
        void generateHeights()
        {
            mNoise.generateNoiseImage();
            mNoiseValues = mNoise.getTextureDataF();
        };
        void computeTerrain()
        {
            generateHeights();
            build();
            setBuffers();
        };
        void genHeightMapTexture();
        void linkHeightMapTexture(GLint shader);

        void setVPMatrix(glm::mat4 vp);
        void enableNormals() {mUseNormals = true; mFloatsPerVertex += 3;};
        void build();
        void setBuffers();
        void draw();

        float getWidth()
        {
            return mWidth;
        };
        float getHeight()
        {
            return mHeight;
        }
        glm::ivec2 getTerrainDimension()
        {
            return glm::ivec2(mWidth, mHeight);
        };
        glm::ivec2 getHeightMapDimension()
        {
            return glm::ivec2(mNoise.getDimension());
        };
        float getHeightMapValue(glm::ivec2 coordinate)
        {
            return mNoiseValues[(coordinate.y * (mNoise.getDimension()).x) + coordinate.x];
        };
        float getTerrainHeight(glm::ivec2 coordinate)
        {
            return mTerrainOffset + mTerrainScale * mNoiseValues[(coordinate.y * mWidth) + coordinate.x];
        };

        float* getNoiseValues()
        {
            return mNoiseValues;
        };
        /* float* getHeightValues() */
        /* { */
        /*     return mNoiseValues; */
        /* }; */
        /* float* getTerrainValues() */
        /* { */
        /*     return mNoiseValues; */
        /* }; */


        void saveNoiseToFile()
        {
            mNoise.saveToFile("terrainNoise.tga");
        }


        void debug();

    private:
        int getSize();
        int getVerticeNumber();
        int getIndicesPerRow();
        int getDegensRequired();
        int getTotalIndices();

        int getIndexLeftPosition(int index);
        int getIndexRightPosition(int index);
        int getIndexTopPosition(int index);
        int getIndexBottomPosition(int index);
        int getIndexTopRightPosition(int index);
        int getIndexBottomLeftPosition(int index);
        glm::vec3 computePosition(int x, int y);
        int computeTerrainPositions();
        //glm::vec3 computeNormals();
        void computeTerrainNormals(int &offset, int &length);

        void buildVBO();
        void buildIBO();

        int mWidth, mHeight;
        int mTotalIndices, mTotalVertices;

        Noise mNoise;
        float* mNoiseValues;

        Texture mHeightMapTexture;
        bool mUseHeightMapTexture;

        int mHeightMapTerrainRatio;
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
        GLuint mVPLocation;
        glm::mat4 mVPMatrix;
        GLuint mTerrainSizeLocation;
        glm::vec2 mTerrainSize;

        /* float minPos, posRange; */
        typedef struct
        {
            glm::vec3 a;
            glm::vec3 b;
            glm::vec3 c;
        } Triangle;
        glm::vec3 computeTriangleNormal(Triangle &t)
        {
            return glm::cross(glm::vec3(t.a - t.c), glm::vec3(t.b - t.c));
        };
        glm::vec3 computeVertexNormal(glm::vec3 v1, glm::vec3 v2, glm::vec3 v3, glm::vec3 v4, glm::vec3 v5, glm::vec3 v6 )
        {
            return glm::normalize(v1 + v2 + v3 + v4 + v5 + v6);
        };

};
#endif /* TERRAIN_H_ */
