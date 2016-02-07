#include "AmbientOcclusion.h"

AmbientOcclusion::AmbientOcclusion(const GLint& occlusionProgram, const GLint& blurProgram, DeferredShading *const deferredShading, Camera *const camera)
    : mOcclusionProgram(occlusionProgram)
    , mBlurProgram(blurProgram)
    , mColorTexture()
    , mBlurTexture()
    , mResolutionX(1278)
    , mResolutionY(986)
{
    this->mDeferredShading = deferredShading;
    this->mCamera = camera;
    /* std::vector<glm::vec3> lightPositions; */
    /* std::vector<glm::vec3> lightColors; */
}

AmbientOcclusion::~AmbientOcclusion()
{}

void AmbientOcclusion::init()
{

    glGenFramebuffers(1, &mColorFBO); 
    glGenFramebuffers(1, &mBlurFBO);

    glBindFramebuffer(GL_FRAMEBUFFER, mColorFBO);
    mColorTexture.bind();
    mColorTexture.setResolution(mResolutionX, mResolutionY);
    mColorTexture.loadAmbientOptions();
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mColorTexture.getTexture(), 0);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "SSAO Framebuffer not complete!" << std::endl;


    glBindFramebuffer(GL_FRAMEBUFFER, mBlurFBO);
    mBlurTexture.bind();
    mBlurTexture.setResolution(mResolutionX, mResolutionY);
    mBlurTexture.loadAmbientOptions();
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mBlurTexture.getTexture(), 0);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "SSAO Blur Framebuffer not complete!" << std::endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);


}

void AmbientOcclusion::linkTextures()
{
    linkTextures(mOcclusionProgram);
}

void AmbientOcclusion::linkTextures(const GLint& shader)
{
    mDeferredShading->linkTextures(mOcclusionProgram);
    mColorTexture.linkTexture(mBlurProgram, "sAmbientColor");
    /* mBlurTexture.linkTexture(mBlurProgram, "sAmbientBlur"); */
    mNoiseTexture.linkTexture(shader, "sNoiseSamples");
}

void AmbientOcclusion::linkBlurredTexture(const GLint& shader)
{
    glUseProgram(shader);
    mBlurTexture.linkTexture(shader, "uAmbientBlur");
    /* ambientBlurShader; */
    /* glActiveTexture(GL_TEXTURE0); */
    /* glBindTexture(GL_TEXTURE_2D, ssaoColorBuffer); */
}

void AmbientOcclusion::initRandomSamples()
{
    std::uniform_real_distribution<GLfloat> randomFloats(0.0, 1.0); // generates random floats between 0.0 and 1.0
    std::default_random_engine generator;
    for (GLuint i = 0; i < 64; ++i)
    {
        glm::vec3 sample(randomFloats(generator) * 2.0 - 1.0, randomFloats(generator) * 2.0 - 1.0, randomFloats(generator));
        sample = glm::normalize(sample);
        sample *= randomFloats(generator);
        GLfloat scale = GLfloat(i) / 64.0;

        // Scale samples s.t. they're more aligned to center of kernel
        scale = 0.1 + (scale*scale*0.9f); //lerp(0.1f, 1.0f, scale * scale);
        sample *= scale;
        mRandSamples.push_back(sample);
    }

    for (GLuint i = 0; i < 16; i++)
    {
        glm::vec3 noise(randomFloats(generator) * 2.0 - 1.0, randomFloats(generator) * 2.0 - 1.0, 0.0f); // rotate around z-axis (in tangent space)
        mNoiseSamples.push_back(noise);
    }

    mNoiseTexture.bind();
    mNoiseTexture.setResolution(4, 4);
    mNoiseTexture.loadAmbientNoiseOptions(mNoiseSamples);
}

void AmbientOcclusion::bindBlurFBO()
{
    glBindFramebuffer(GL_FRAMEBUFFER, mBlurFBO);
}

void AmbientOcclusion::bindColorFBO()
{
    glBindFramebuffer(GL_FRAMEBUFFER, mColorFBO);
}

void AmbientOcclusion::bindColorTexture()
{
    mColorTexture.bind();
}

void AmbientOcclusion::bindBlurredTexture()
{
    mBlurTexture.bind();
}

void AmbientOcclusion::bindNoiseTexture()
{
    mNoiseTexture.bind();
}

void AmbientOcclusion::bindTextures()
{
    /* mGPosition.bind(); */
    /* mGNormal.bind(); */
    /* mGAlbedo.bind(); */
    /* glUseProgram(mAmbientOcclusionProgram); */
    mColorTexture.bind();
    mBlurTexture.bind();
    mNoiseTexture.bind();
}

void AmbientOcclusion::bindDeferredTextures()
{
    mDeferredShading->bindTextures();
}

void AmbientOcclusion::loadUniforms()
{
    loadUniforms(mOcclusionProgram);
}

void AmbientOcclusion::loadUniforms(const GLint& shader)
{
    for (GLuint i = 0; i < 64; ++i)
        glUniform3fv(glGetUniformLocation(shader, ("samples[" + std::to_string(i) + "]").c_str()), 1, &mRandSamples[i][0]);

    glUniformMatrix4fv(glGetUniformLocation(shader, "uPMatrix"), 1, GL_FALSE, glm::value_ptr(mCamera->getProjectionMatrix()));
}
