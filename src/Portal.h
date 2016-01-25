#ifndef PORTAL_H
#define PORTAL_H

#include <random>
#include "Camera.h"
#include "Shader.h"
#include "ModelLoader.h"
#include "Terrain.h"

class Portal
{
public:
    Portal(const GLint& mShaderProgram);
    void init( Camera *const cam, Terrain *const terrain1, Terrain *const terrain2);
    /* void loadPortal(); */

    void enableStencil();
    void substractStencil();
    void renderOutside();
    void renderInside();
    void disableStencil();

    void drawPortal();
    void drawFill();

    void setScale(glm::vec3 ratio);
    void setTranslation(glm::vec3 dist);
    void setTransforms(glm::vec3 rot, glm::vec3 scale, glm::vec3 trans);
    glm::vec3 getPosition();
    glm::vec3 getPosition2() { return mPosition2; };
    glm::vec3 getRotation2() { return mRot2; };
    bool isActive() { return mStatus; };
    void aktivate() { this->mStatus = true; };
    void deaktivate() { teleport(); };

    glm::mat4 getNextMVP() {return mMVP2;};
    glm::mat4 getNextVP() {return mVP2;};
    glm::mat3 getNextInvV() {return mIV2;};

    void teleport();
    /* void setHeight(const float& height); */

    /* void drawPortal( const Shader& shaderProg, const Camera& camera ); */
    /* void drawFill( const Shader& shaderProg, const Camera& camera ); */
    /* void drawPortal( const Shader& shaderProg, */
    /*                   const Camera& camera, */
    /*                   const glm::vec3& translation, */
    /*                   const glm::vec3& rotation, */
    /*                   const glm::vec3& scaling); */
    /* void drawFill( const Shader& shaderProg, */
    /*                   const Camera& camera, */
    /*                   const glm::vec3& translation, */
    /*                   const glm::vec3& rotation, */
    /*                   const glm::vec3& scaling); */
private:
    static unsigned int idGenerator;
    unsigned mId;
    bool mStatus;
    glm::vec3 mPosition1;
    glm::vec3 mPosition2;
    glm::vec3 mRot2;
    glm::vec3 mScale2;
    glm::vec3 mTrans2;
    glm::mat4 mMVP2;
    glm::mat4 mVP2;
    glm::mat3 mIV2;
    ModelLoader *model;
    ModelLoader *modelFill;
    Camera* camera;
    glm::vec3 mTmpCamPos;

    /* void drawFill(); */
    /* void drawPortal(); */
    void enableRenderStencilPattern();
    void disableRenderStencilPattern();
};

#endif // PORTAL_H
