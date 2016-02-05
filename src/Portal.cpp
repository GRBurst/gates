#include "Portal.h"
unsigned int Portal::idGenerator = 0;

Portal::Portal(const GLint& mShaderProgram)
{
    this->mId = idGenerator++;
    this->mStatus = true;

    model = new ModelLoader("../objects/flat_torus.obj", mShaderProgram);
    modelFill = new ModelLoader("../objects/inner_plane.obj", mShaderProgram);

    /* std::cout << "new Portal id = " << mId << std::endl; */

}

Portal::~Portal()
{
    delete model;
    delete modelFill;
}

void Portal::init( Camera *const cam, Terrain *const activeTerrain, Terrain *const inactiveTerrain )
{

    /* std::cout << "load model fill files" << std::endl; */
    // Setup model data
    modelFill->loadFile();
    modelFill->setBuffers();
    modelFill->setStandardUniformLocations();

    /* std::cout << "load model files" << std::endl; */
    model->loadFile();
    model->setBuffers();
    model->setStandardUniformLocations();

    /* std::cout << "set camera" << std::endl; */
    // Set camera
    camera = cam;

    /* std::cout << "generate new positions" << std::endl; */
    // Generate positions
    std::default_random_engine gen(mId + 42);
    std::uniform_int_distribution<int> hDim1(0, activeTerrain->getWidth());
    std::uniform_int_distribution<int> vDim1(0, activeTerrain->getHeight());
    std::uniform_real_distribution<double> deg(0.0, 360.0);

    /* std::cout << "generate new x and y locations" << std::endl; */
    int x = hDim1(gen);
    int y = vDim1(gen);
    /* std::cout << "calc active terrain locations: x = " << x << ", y = " << y << std::endl; */
    mPosition1 = glm::vec3(0.0, 0.0, 0.0) + activeTerrain->computePosition(x, y);
    /* setRotation(glm::vec3(0.0, deg(gen), 0.0)); */
    setScale(glm::vec3(1.0, 1.0, 1.0));
    setTranslation(mPosition1);

    std::uniform_int_distribution<int> hDim2(0, inactiveTerrain->getWidth());
    std::uniform_int_distribution<int> vDim2(0, inactiveTerrain->getHeight());

    /* std::cout << "calc inactive terrain locations" << std::endl; */
    mPosition2 = glm::vec3(0.0, 1.0, 0.0) + inactiveTerrain->computePosition(hDim2(gen), vDim2(gen));
    mScale2 = glm::vec3(1.0, 1.0, 1.0);
    mTrans2 = mPosition2 - mPosition1;
    mRot2 = glm::vec3(0.0, deg(gen), 0.0);

    /* std::cout << "Portal " << mId << " coords 1: x = " << mPosition1.x << ", y = " << mPosition1.y << ", z = " << mPosition1.z << std::endl; */
    /* std::cout << "Portal " << mId << " coords 2: x = " << mPosition2.x << ", y = " << mPosition2.y << ", z = " << mPosition2.z << std::endl; */

}

glm::vec3 Portal::getPosition()
{
    if(mStatus) return mPosition1;
    return mPosition2;
}

void Portal::setScale(glm::vec3 ratio)
{
    model->scale(ratio);
    modelFill->scale(ratio);
}

void Portal::setTranslation(glm::vec3 dist)
{
    model->translate(dist);
    modelFill->translate(dist);
}

void Portal::setTransforms(glm::vec3 rot, glm::vec3 scale, glm::vec3 trans)
{
    /* setRotation(rot); */
    setScale(scale);
    setTranslation(trans);
}

// Fist call: Save masks
void Portal::enableStencil()
{
    glGetBooleanv(GL_COLOR_WRITEMASK, mColorMask);
    glGetBooleanv(GL_DEPTH_WRITEMASK, &mDepthMask);

}

// Secound call: Render current scene/terrain
void Portal::renderOutside()
{
    modelFill->setProjection(camera->getProjectionMatrix());
    modelFill->setView(camera->getViewMatrix());

    glStencilFunc(GL_ALWAYS, 0, 0xFF);
    glStencilOp(GL_ZERO, GL_ZERO, GL_ZERO);
    glStencilMask(0xFF);
}

// Third call: Render next scene/terrain in the stencil
void Portal::renderInside()
{
    glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
    glDepthMask(GL_FALSE);
    glStencilFunc(GL_ALWAYS, 0, 0xFF);
    glStencilOp(GL_KEEP, GL_KEEP, GL_INCR);
    glStencilMask(0xFF);

    drawFill();

    glColorMask(mColorMask[0], mColorMask[1], mColorMask[2], mColorMask[3]);
    glDepthMask(mDepthMask);
    glStencilFunc(GL_EQUAL, 1, 0xFF);
    glStencilMask(0x00);
    glClear( GL_DEPTH_BUFFER_BIT );

    mTmpCamPos = camera->getPosition();
    mTmpNearPlane = camera->getNearPlane();
    camera->setPosition(mPosition2-(mPosition1-mTmpCamPos));
    camera->setNearPlane(glm::length(mPosition1-mTmpCamPos));
    /* camera->rotate(glm::vec2(pActivePortal->getRotation2(), 0.0)); */
    camera->update();
}

// Fourth call: Disable stencil rendering. Continue with post rendering (shadow maps or something like this
void Portal::disableStencil()
{
    camera->setPosition(mTmpCamPos);
    camera->setNearPlane(mTmpNearPlane);
    camera->update();
    glStencilMask(0xFF);
}


/* void Portal::drawPortal( const ConstSharedShaderProgram& shaderProg, const GenericCamera& camera ){ */
void Portal::drawPortal()
{
    /* if(!mStatus) std::cout << "render inactve portal at: x = " << std::endl; */
    model->setProjection(camera->getProjectionMatrix());
    model->setView(camera->getViewMatrix());
    model->draw();
}

void Portal::drawFill()
{
    modelFill->setProjection(camera->getProjectionMatrix());
    modelFill->setView(camera->getViewMatrix());
    modelFill->draw();
}

void Portal::teleport()
{
    this->mStatus = false;
    /* setRotation(mRot2); */
    setScale(mScale2);
    setTranslation(mTrans2);
    /* std::cout << std::endl << "Portal " << mId << " teleportation" << std::endl; */
}

