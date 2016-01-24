#include "Portal.h"
unsigned int Portal::idGenerator = 0;

Portal::Portal(const GLint& mShaderProgram)
{
    this->mId = idGenerator++;
    this->mStatus = true;

    model = new ModelLoader("../objects/flat_torus.obj", mShaderProgram);
    modelFill = new ModelLoader("../objects/inner_plane.obj", mShaderProgram);

}

void Portal::init( Camera *const cam, Terrain *const terrain1, Terrain *const terrain2)
{

    modelFill->loadFile();
    modelFill->setBuffers();
    modelFill->setStandardUniformLocations();

    model->loadFile();
    model->setBuffers();
    model->setStandardUniformLocations();

    camera = cam;

    std::default_random_engine gen(mId);
    std::uniform_int_distribution<int> hDim1(0, terrain1->getWidth());
    std::uniform_int_distribution<int> vDim1(0, terrain1->getHeight());
    std::uniform_real_distribution<double> deg(0.0, 360.0);

    /* mPosition1 = terrain1->computePosition(hDim1(gen), vDim1(gen)); */
    mPosition1 = glm::vec3(5.0, 1.0, 5.0);
    /* setRotation(glm::vec3(0.0, deg(gen), 0.0)); */
    setScale(glm::vec3(1.0, 1.0, 1.0));
    /* setTranslation(mPosition); */
    setTranslation(glm::vec3(5.0, 1.0, 5.0));

    std::uniform_int_distribution<int> hDim2(0, terrain2->getWidth());
    std::uniform_int_distribution<int> vDim2(0, terrain2->getHeight());

    /* mPosition2 = terrain2->computePosition(hDim2(gen), vDim2(gen)); */
    mPosition2 = glm::vec3(10.0, 1.0, 5.0);
    mScale2 = glm::vec3(1.0, 1.0, 1.0);
    mTrans2 = glm::vec3(5.0, 0.0, 0.0);
    mRot2 = glm::vec3(0.0, deg(gen), 0.0);

    Camera cam2;
    cam2.setPosition(mPosition2);
    cam2.rotate(glm::vec2(mRot2.y, 0.0));
    cam2.update();
    mMVP2 = cam2.getVPMatrix();
    mVP2 = cam2.getVPMatrix();
    mIV2 = cam2.getInvViewMatrix();

}

/* void setHeight(const float& height); */

/* void Portal::setRotation(glm::vec3 angle) */
/* { */
/*     model->rotate(angle); */
/*     modelFill->rotate(angle); */
/* } */

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

void Portal::enableRenderStencilPattern()
{
    //Settings for init rendering coords for stencil buffer
    glEnable(GL_STENCIL_TEST);
    /* glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE); */
    /* glDepthMask(GL_FALSE); */
    glStencilFunc(GL_NEVER, 1, 0xFF);
    glStencilOp(GL_REPLACE, GL_KEEP, GL_KEEP);  // draw 1s on test fail (always)

    // draw stencil pattern
    glStencilMask(0xFF);
    glClear(GL_STENCIL_BUFFER_BIT);  // clear stencil buffer . needs mask=0xFF

}

void Portal::disableRenderStencilPattern()
{
    /* glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE); */
    /* glDepthMask(GL_TRUE); */
    glStencilMask(0x00);
}

/* void Portal::substractStencil() */
/* { */
/*     glStencilFunc(GL_EQUAL, 1, 0xFF);   // drawing the scene . skip where stencil's value is 0 */
/*     glStencilOp(GL_KEEP, GL_ZERO, GL_KEEP);  // draw 1s on test fail (always) */
/*     glStencilMask(0xFF); */
/* } */

// Fist call: Renders stencil pattern
void Portal::enableStencil()
{
    /* portal.draw( shaderProg , camera , glm::dvec3(pGame.portalPosition[pGame.portalIndicator]) ); */
    enableRenderStencilPattern();
    modelFill->setProjection(camera->getProjectionMatrix());
    modelFill->setView(camera->getViewMatrix());
    if(mStatus)  modelFill->draw();
    disableRenderStencilPattern();
}

// Secound call: Render current scene/terrain around stencil
void Portal::renderOutside()
{
    /* disableRenderStencilPattern(); */
    glStencilFunc(GL_EQUAL, 0, 0xFF);   // drawing the scene . skip where stencil's value is 0
    drawPortal();
}

// Third call: Render next scene/terrain in the stencil
void Portal::renderInside(){
    // drawing the scene in coords where we filled the buffer . skip where stencil's value is != 0
    glStencilFunc(GL_EQUAL, 1, 0xFF);
}

// Fourth call: Disable stencil rendering. Continue with post rendering (shadow maps or something like this
void Portal::disableStencil()
{
    glDisable(GL_STENCIL_TEST);
}


/* void Portal::drawPortal( const ConstSharedShaderProgram& shaderProg, const GenericCamera& camera ){ */
void Portal::drawPortal()
{
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

void Portal::worb()
{
    this->mStatus = false;
    /* setRotation(mRot2); */
    setScale(mScale2);
    setTranslation(mTrans2);
    std::cout << std::endl << "Portal worb" << std::endl;
}
/* void Portal::draw(const ConstSharedShaderProgram& shaderProg, */
/*                       const GenericCamera& camera, */
/*                       const dvec3& translation, */
/*                       const dvec3& rotation, */
/*                       const dvec3& scaling) const { */
        /* model.draw(shaderProg, camera, */
        /*      translate( mat4(1.0f), vec3(translation) ) */
        /*      * rotate( mat4(1.0f), static_cast<float>(rotation.z), vec3(0.0, 0.0, 1.0) ) */
        /*      * rotate( mat4(1.0f), static_cast<float>(rotation.y), vec3(0.0, 1.0, 0.0) ) */
        /*      * rotate( mat4(1.0f), static_cast<float>(rotation.x), vec3(1.0, 0.0, 0.0) ) */
        /*      * scale( mat4(1.0f), vec3(scaling) )); */
    /* } */

/* void Portal::drawFill(const ConstSharedShaderProgram& shaderProg, */
/*                       const GenericCamera& camera, */
/*                       const dvec3& translation, */
/*                       const dvec3& rotation, */
/*                       const dvec3& scaling) const { */
        /* model.drawFill(shaderProg, camera, */
        /*      translate( mat4(1.0f), vec3(translation) ) */
        /*      * rotate( mat4(1.0f), static_cast<float>(rotation.z), vec3(0.0, 0.0, 1.0) ) */
        /*      * rotate( mat4(1.0f), static_cast<float>(rotation.y), vec3(0.0, 1.0, 0.0) ) */
        /*      * rotate( mat4(1.0f), static_cast<float>(rotation.x), vec3(1.0, 0.0, 0.0) ) */
        /*      * scale( mat4(1.0f), vec3(scaling) )); */
    /* } */

