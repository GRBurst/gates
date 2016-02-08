#ifndef AMBIENT_OCCLUSION_H_
#define AMBIENT_OCCLUSION_H_
#include <GL/glew.h>
#include <iostream>
#include <stdio.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GLFW/glfw3.h>
#include <vector>
#include <random>
#include "Camera.h"
#include "Texture.h"
#include "DeferredShading.h"

/*
 * Usage:
 * 1.) Construct
 * 2.) Add lights
 * 3.) Render to texture
 * 4.) Render lights
 */
class AmbientOcclusion {

    public:
        AmbientOcclusion(const GLint& occlusionProgram, const GLint& blurProgram, DeferredShading *const deferredShading, Camera *const camera);
        ~AmbientOcclusion();

        void init();
        void linkTextures();
        void linkTextures(const GLint& shader);
        void linkBlurredTexture(const GLint& shader);
        void bindColorFBO();
        void bindBlurFBO();
        void bindTextures();
        void bindNoiseTexture();
        void bindColorTexture();
        void bindBlurredTexture();
        void bindDeferredTextures();
        void loadUniforms();
        void loadUniforms(const GLint& shader);

        void initRandomSamples();

        Texture& getColorTexture() { return mColorTexture; };
        Texture& getBlurTexture() { return mBlurTexture; };
        Texture& getNoiseTexture() { return mNoiseTexture; };

    private:
        Camera* mCamera;
        DeferredShading* mDeferredShading;
        GLint mOcclusionProgram, mBlurProgram;
        GLuint mColorFBO, mBlurFBO;
        Texture mColorTexture, mBlurTexture, mNoiseTexture;
        int mResolutionX, mResolutionY;
        GLuint mKernelSize;

        std::vector<glm::vec3> mRandSamples;
        std::vector<glm::vec3> mNoiseSamples;

};
#endif /* AMBIENT_OCCLUSION_H_ */
