#ifndef DEFERRED_SHADING_H_
#define DEFERRED_SHADING_H_
#include <GL/glew.h>
#include <iostream>
#include <stdio.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GLFW/glfw3.h>
#include <vector>
#include "Camera.h"
#include "Terrain.h"
#include "Texture.h"

/*
 * Usage:
 * 1.) Construct
 * 2.) Add lights
 * 3.) Render to texture
 * 4.) Render lights
 */
class DeferredShading {

    public:
        DeferredShading(const GLint& gBufferProgram, const GLint& defferedShadingProgram, Camera *const camera);
        ~DeferredShading();

        void init();
        /* void resize(); */
        void linkTextures();
        void linkTextures(const GLint& shader);
        void bindFBO();
        void bindTextures();
        void loadUniforms(Terrain *const terrain);
        void loadUniforms(const GLint& shader, Terrain *const terrain);
        void copyDepthBuffer(const GLuint& destinationFBO = 0);

        void addLight();
        void initLights();
        void initRandomLights();

        void renderToTexture();
        void renderLights();

        Texture& getPositionTexture() { return mGPosition; };
        Texture& getNormalTexture() { return mGNormal; };
        Texture& getAlbedoTexture() { return mGAlbedo; };

    private:
        GLint mGBufferProgram, mDeferredShadingProgram;
        GLuint mGBuffer, mGDepth;
        Camera* mCamera;
        Texture mGPosition, mGNormal, mGAlbedo;
        GLuint mResolutionX, mResolutionY;

        int mNumLights;
        std::vector<glm::vec3> lightPositions;
        std::vector<glm::vec3> lightColors;

};
#endif /* DEFERRED_SHADING_H_ */
