#ifndef TERRAIN_H_
#define TERRAIN_H_
#include <GL/glew.h>
#include <iostream>
#include <stdio.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GLFW/glfw3.h>
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
        void addNoise(const std::vector<float>& noiseValues);
        void multiplyNoise(const std::vector<float>& noiseValues);
        void build();

        void genHeightMapTexture();
        void linkHeightMapTexture(GLint shader);

        void enableNormals();
        void enableNormalMap();
        void draw();
        void drawReflection();
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
        float getHeightOfPosition(const glm::vec3& position) const;

        glm::vec3 computePosition(unsigned int x, unsigned int y) const;
        void modifyHeight();
        void highlightRay(const glm::vec3& ray, unsigned int& mode);
        void unHighlightRay() { this->mEditMode = 2.0; };
        void changeModifyRadius(const double& yoffset);
        float getIndexDistances(const glm::ivec2& p1, const glm::ivec2& p2) const;

        /* std::vector<float> getNoiseValues() const { return mNoiseValues; }; */
        float* getNoiseValues() { return mNoiseValues.data(); };
        void setNoiseValues(std::vector<float>& noiseVal){mNoiseValues = noiseVal; };

        void setGrid(int mode) { mDrawGrid = mode; };
        void saveNoiseToFile(const char* filename = "terrainNoise.tga") const;
        void loadWater3DNoise( Noise* noise);
        void loadWaterNormal3DNoise( Noise* noise);
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
        unsigned int getIndexFromPosition( const glm::ivec2& pos ) const;

        void computeTerrainPositions();
        void computeTerrainNormals();
        void computeTerrainNormals(unsigned int min, unsigned int max);
        void computeTerrainTangents();
        void computeTerrainTangents(unsigned int min, unsigned int max);

        void setBuffers();
        void buildVBO();
        void buildIBO();
        void buildDebug();

        void updateArea( );
        void updateVertexBuffer(unsigned int offset, unsigned int size, float* newVertexData);
        glm::ivec2 getIntersectionPoint(const glm::vec3& position, const glm::vec3& ray);

        // Member variables
        unsigned int mXDim, mZDim;
        unsigned int mTotalIndices, mTotalStripIndices, mTotalVertices;

        Camera* mCamera;
        Noise* mNoise;
        /* float* mNoiseValues = NULL; */
        std::vector<float> mNoiseValues;

        Texture mHeightMapTexture;
        bool mUseHeightMapTexture;
        Texture mNormalMapTexture;
        Texture mWhiteNoiseTexture;

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
        GLuint muVLocation;
        glm::mat4 mVMatrix;
        glm::mat4 mMMatrix;
        GLuint muInvViewLocation;
        glm::mat3 mInvViewMatrix;
        GLint muHeightMapTerrainRatioLocation;
        unsigned int mHeightMapTerrainRatio;
        GLint muDrawGridLocation;
        int mDrawGrid;
        glm::vec3 mRayTerrainIntersection;
        GLint muRayTerrainIntersectionLocation;
        float mModifyRadius, mEditMode;
        GLint muModifyRadius, muEditLocation;
        glm::ivec2 mIntersectionCoords;
        glm::vec3 mCamPos;
        GLint muCamPosLocation;
        glm::vec3 mLightPos;
        GLint muLightPosLocation;
        GLint muTimeLocation;
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
