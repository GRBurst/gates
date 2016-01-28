#ifndef TERRAIN_H_
#define TERRAIN_H_
#include <GL/glew.h>
#include <iostream>
#include <stdio.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Camera.h"
#include "Noise.h"
#include "Texture.h"

/*
 * Usage:
 * 1.) Construct Terrain();
 * 2.) Optional: enableNormals()
 * 3.) computeTerrain()
 * 4.) Optional: genHeightMapTexture()
 * 5.) draw()
 */
class Terrain {

    public:
        Terrain(GLint shaderProgram, unsigned int xDim, unsigned int zDim, Camera *const camera, Noise *const noise);
        ~Terrain();

        // Control functions
        void generateHeights();
        void computeTerrain();
        void genHeightMapTexture();
        void linkHeightMapTexture(GLint shader);

        void enableNormals();
        void enableNormalMap();
        void draw();

        // Getters / setters
        unsigned int getWidth() const { return mXDim; };
        unsigned int getHeight() const { return mZDim; };
        unsigned int getTotalIndices() const { return mTotalIndices; };
        glm::ivec2 getTerrainDimension() const { return glm::ivec2(mXDim, mZDim); };
        glm::ivec2 getHeightMapDimension() const { return glm::ivec2(mNoise->getDimension()); };
        GLuint getVAO() const { return mVao; };

        float getHeightMapValue(const glm::ivec2& coordinate) const;
        float getTerrainHeight(const glm::ivec2& coordinate) const;
        glm::vec3 getTerrainPosition(const glm::ivec2& coordinate) const;
        glm::vec2 getIndexCordFromTerrain(const glm::vec3& position) const;

        glm::vec3 computePosition(unsigned int x, unsigned int y) const;
        void getRayTerrainIntersection(glm::vec3& ray);

        /* std::vector<float> getNoiseValues() const { return mNoiseValues; }; */
        float* getNoiseValues() { return mNoiseValues.data(); };
        void setNoiseValues(std::vector<float>& noiseVal){mNoiseValues = noiseVal; };

        void setGrid(int mode) { mDrawGrid = mode; };
        void saveNoiseToFile(const char* filename = "terrainNoise.tga") const;

        void debug();

    private:
        unsigned int getSize() const;
        unsigned int getVerticeNumber() const;
        unsigned int getIndicesPerRow() const;
        unsigned int getDegensRequired() const;

        unsigned int getIndexLeftPosition(unsigned int index) const;
        unsigned int getIndexRightPosition(unsigned int index) const;
        unsigned int getIndexTopPosition(unsigned int index) const;
        unsigned int getIndexBottomPosition(unsigned int index) const;
        unsigned int getIndexTopRightPosition(unsigned int index) const;
        unsigned int getIndexBottomLeftPosition(unsigned int index) const;

        void computeTerrainPositions();
        void computeTerrainNormals();
        void computeTerrainTangents();

        void build();
        void setBuffers();
        void buildVBO();
        void buildIBO();
        void buildDebug();

        void updateArea(glm::ivec2& bl, glm::ivec2& tl, glm::ivec2& br, glm::ivec2& tr );
        void updateVertexBuffer(int offset, int size, float* newVertexData);

        // Member variables
        unsigned int mXDim, mZDim;
        unsigned int mTotalIndices, mTotalStripIndices, mTotalVertices;

        Camera* mCamera;
        Noise* mNoise;
        /* float* mNoiseValues = NULL; */
        std::vector<float> mNoiseValues;
        NoiseType noiseType;

        Texture mHeightMapTexture;
        bool mUseHeightMapTexture;

        float mTerrainOffset;
        float mTerrainScale;
        float mTerrainMinPos;
        float mTerrainPosRange;
        bool mUseNormals;
        bool mUseNormalMap;

        /* GLfloat* mVertices = NULL; */
        /* GLint* mIndices = NULL; */
        std::vector<GLfloat> mVertices;
        /* std::vector<glm::vec3> mVertices; */
        std::vector<GLint> mIndices;
        std::vector<GLint> mIndicesStrip;
        unsigned int mFloatsPerVertex;
        unsigned int mElementsPerVertex;
        GLuint mVao;
        GLuint mVbo;
        GLuint mIbo;

        GLint mShaderProgram;
        GLuint muVPLocation;
        glm::mat4 mVPMatrix;
        GLuint muInvViewLocation;
        glm::mat3 mInvViewMatrix;
        GLint muHeightMapTerrainRatioLocation;
        unsigned int mHeightMapTerrainRatio;
        GLint muDrawGridLocation;
        int mDrawGrid;
        glm::vec3 mRayTerrainIntersection;
        GLint muRayTerrainIntersectionLocation;
        glm::vec3 mCamPos;
        GLint muCamPos;

        /* float minPos, posRange; */
        typedef struct
        {
            glm::vec3 a;
            glm::vec3 b;
            glm::vec3 c;
        } Triangle;
        // normal, counterclockwise edges
        glm::vec3 computeTriangleNormal(Triangle &t);
        glm::vec3 computeVertexNormal(glm::vec3 v1, glm::vec3 v2, glm::vec3 v3, glm::vec3 v4, glm::vec3 v5, glm::vec3 v6 );

};
#endif /* TERRAIN_H_ */
