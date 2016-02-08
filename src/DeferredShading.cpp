#include "DeferredShading.h"

DeferredShading::DeferredShading(const GLint& gBufferProgram, const GLint& deferredShadingProgram, Camera *const camera)
    : mGBufferProgram(gBufferProgram)
    , mDeferredShadingProgram(deferredShadingProgram)
    , mGBuffer(0)
    , mGDepth(0)
    /* , mCamera(camera) */
    , mGPosition()
    , mGNormal()
    , mGAlbedo()
    , mResolutionX(1280)
    , mResolutionY(1024)
    , mNumLights(32)
{
    this->mCamera = camera;
    /* std::vector<glm::vec3> lightPositions; */
    /* std::vector<glm::vec3> lightColors; */
}

DeferredShading::~DeferredShading()
{}

void DeferredShading::init()
{

    glGenFramebuffers(1, &mGBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, mGBuffer);

    mGPosition.bind();
    mGPosition.setResolution(mResolutionX, mResolutionY);
    mGPosition.loadGDepthPositionOptions();
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mGPosition.getTexture(), 0);

    mGNormal.bind();
    mGNormal.setResolution(mResolutionX, mResolutionY);
    mGNormal.loadGNormalOptions();
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, mGNormal.getTexture(), 0);

    mGAlbedo.bind();
    mGAlbedo.setResolution(mResolutionX, mResolutionY);
    mGAlbedo.loadGAlbedoOptions();
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, mGAlbedo.getTexture(), 0);

    /* glGenTextures(1, &gAlbedoSpec); */
    /* glBindTexture(GL_TEXTURE_2D, gAlbedoSpec); */
    /* glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, mResolutionX, mResolutionY, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL); */
    /* glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); */
    /* glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); */
    /* glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D, gAlbedoSpec, 0); */

    GLuint drawBuffers[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
    glDrawBuffers(3, drawBuffers);

    glGenRenderbuffers(1, &mGDepth);
    glBindRenderbuffer(GL_RENDERBUFFER, mGDepth);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, mResolutionX, mResolutionY);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, mGDepth);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "Framebuffer not complete!" << std::endl;
}

void DeferredShading::linkTextures()
{
    linkTextures(mDeferredShadingProgram);
}

void DeferredShading::linkTextures(const GLint& shader)
{

    /* glUseProgram(mDeferredShadingShaderProgram); */
    /* glUniform1i(glGetUniformLocation(shader, "uGPosition"), 10); */
    /* glUniform1i(glGetUniformLocation(shader, "uGNormal"), 11); */
    /* glUniform1i(glGetUniformLocation(shader, "uGAlbedo"), 12); */
    mGPosition.linkTexture(shader, "uGPosition");
    mGNormal.linkTexture(shader, "uGNormal");
    mGAlbedo.linkTexture(shader, "uGAlbedo");
}


void DeferredShading::initRandomLights()
{

    srand(100);
    for (int i = 0; i < mNumLights; i++)
    {
        // Calculate slightly random offsets
        GLfloat xPos = ((rand() % 50)) - 25.0;
        GLfloat yPos = 10.0;
        GLfloat zPos = ((rand() % 50)) - 25.0;
        lightPositions.push_back(glm::vec3(xPos, yPos, zPos));
        // Also calculate random color
        GLfloat rColor = ((rand() % 100) / 200.0f) + 0.5; // Between 0.5 and 1.0
        GLfloat gColor = ((rand() % 100) / 200.0f) + 0.5; // Between 0.5 and 1.0
        GLfloat bColor = ((rand() % 100) / 200.0f) + 0.5; // Between 0.5 and 1.0
        lightColors.push_back(glm::vec3(rColor, gColor, bColor));
    }
}

void DeferredShading::bindFBO()
{
    glBindFramebuffer(GL_FRAMEBUFFER, mGBuffer);
}

void DeferredShading::bindTextures()
{
    mGPosition.bind();
    mGNormal.bind();
    mGAlbedo.bind();
}

void DeferredShading::loadUniforms(Terrain *const terrain)
{
    loadUniforms(mGBufferProgram, terrain);
}

void DeferredShading::loadUniforms(const GLint& shader, Terrain *const terrain)
{
    glUseProgram(shader);
    // Also send light relevant uniforms
    for (GLuint i = 0; i < lightPositions.size(); i++)
    {
        glUniform3fv(glGetUniformLocation(shader, ("lights[" + std::to_string(i) + "].Position").c_str()), 1, &lightPositions[i][0]);
        glUniform3fv(glGetUniformLocation(shader, ("lights[" + std::to_string(i) + "].Color").c_str()), 1, &lightColors[i][0]);
        // Update attenuation parameters and calculate radius
        const GLfloat constant = 1.0; // Note that we don't send this to the shader, we assume it is always 1.0 (in our case)
        const GLfloat linear = 0.7;
        const GLfloat quadratic = 1.8;
        glUniform1f(glGetUniformLocation(shader, ("lights[" + std::to_string(i) + "].Linear").c_str()), linear);
        glUniform1f(glGetUniformLocation(shader, ("lights[" + std::to_string(i) + "].Quadratic").c_str()), quadratic);
        // Then calculate radius of light volume/sphere
        const GLfloat lightThreshold = 5.0; // 5 / 256
        const GLfloat maxBrightness = std::fmaxf(std::fmaxf(lightColors[i].r, lightColors[i].g), lightColors[i].b);
        GLfloat radius = (-linear + static_cast<float>(std::sqrt(linear * linear - 4 * quadratic * (constant - (256.0 / lightThreshold) * maxBrightness)))) / (2 * quadratic);
        glUniform1f(glGetUniformLocation(shader, ("lights[" + std::to_string(i) + "].Radius").c_str()), radius);
    }

    glUniform3fv(glGetUniformLocation(shader, "uCamPos"), 1, &(mCamera->getPosition()[0]));
    glUniform3fv(glGetUniformLocation(shader, "uRayTerrainIntersection"), 1, &(terrain->getRayTerrainIntersection()[0]));
    glUniform1f(glGetUniformLocation(shader, "uNearPlane"), mCamera->getNearPlane());
    glUniform1f(glGetUniformLocation(shader, "uFarPlane"), mCamera->getFarPlane());
    glUniform1f(glGetUniformLocation(shader, "uEditMode"), terrain->getEditMode());
    glUniform1f(glGetUniformLocation(shader, "uModifyRadius"), terrain->getModifyRadius());
    terrain->loadGBufferMaps(shader);

}

void DeferredShading::copyDepthBuffer(const GLuint& destinationFBO)
{
        // 2.5. Copy content of geometry's depth buffer to default framebuffer's depth buffer
        glBindFramebuffer(GL_READ_FRAMEBUFFER, mGBuffer);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, destinationFBO);
        glBlitFramebuffer(0, 0, mResolutionX, mResolutionY, 0, 0, mResolutionX, mResolutionY, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
}
