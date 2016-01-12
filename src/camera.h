#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera
{

    public:
	    Camera();
	    ~Camera() {};

        // Getters & Setters
        glm::vec3 getCamPos();
        void setCamPos(const glm::vec3 &position);
        glm::vec3 getViewDirection();
        void setViewDirection(const glm::vec3 &viewDirection);
        float getNearPlane();
        void setNearPlane(float nearPlane);
        float getFarPlane();
        void setFarPlane(float farPlane);
        float getFielOfViewHorizontal();
        void setFielOfViewHorizontal(float fieldOfViewHorizontal);
        float getAspectRatio();
        void setAspectRatio(float aspectRatio);

        glm::mat4 getFrustum();
        void setFrustum(glm::mat4 frustum);
        glm::mat4 getViewMatrix();
        void setViewMatrix(glm::mat4 viewMatrix);
        glm::mat4 getProjectionMatrix();
        void setProjectionMatrix(glm::mat4 projectionMatrix);

        // Functions
        glm::mat4 lookAt(   const glm::vec3 &camPos = glm::vec3(0.0f, 0.0f, 0.0f)
                        , const glm::vec3 &viewDirection = glm::vec3(0.0f, 0.0f, -1.0f)
                        , const glm::vec3 &up = glm::vec3(0.0f, 1.0f, 0.0f)
                        );

    private:
	    void buildFrustum(float fov, float ratio, float near, float far);

        glm::vec3   mCamPos;
        glm::vec3   mViewDirection;

        glm::mat4   mFrustum;
        glm::mat4   mViewMatrix;
        glm::mat4   mProjectionMatrix;

        float       mNearPlane;
        float       mFarPlane;
        float       mFieldOfViewHorizontal;
        float       mAspectRatio;


};
