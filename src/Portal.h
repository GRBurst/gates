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
    void init( Camera* cam, const Terrain& terrain );
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
    glm::vec3 mPosition;
    ModelLoader *model;
    ModelLoader *modelFill;
    Camera* camera;

    /* void drawFill(); */
    /* void drawPortal(); */
    void enableRenderStencilPattern();
    void disableRenderStencilPattern();
};

#endif // PORTAL_H
