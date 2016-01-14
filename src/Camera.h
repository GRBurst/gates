#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <math.h>

class Camera
{

    public:
	    Camera();
	    ~Camera() {};

        // Getters & Setters
        glm::vec3 Camera::getPos() const { return this->mPos; };
        void Camera::setPos(const glm::vec3 &position) { this->mPos = position; };

        float Camera::getNearPlane() const { return this->mNearPlane; };
        void Camera::setNearPlane(float nearPlane) { this->mNearPlane = nearPlane; };

        float Camera::getFarPlane() const { return this->mFarPlane; };
        void Camera::setFarPlane(float farPlane) { this->mFarPlane = farPlane; };

        float Camera::getHFoV() const { return this->mHFoV; };
        void Camera::setHFoV(float fov) { this->mHFoV = fov; };

        float Camera::getAspectRatio() const { return this->mAspectRatio; };
        void Camera::setAspectRatio(float aspectRatio) { this->mAspectRatio = aspectRatio; }

        float Camera::getHorizontalAngle() const { return this->mHorizontalAngle; };
        void Camera::setHorizontalAngle(float angle) { this->mHorizontalAngle = angle; }

        float Camera::getVerticalAngle() const { return this->mVerticalAngle; };
        void Camera::setVerticalAngle(float angle) { this->mVerticalAngle = angle; }

        // Calculatable matrizes from camera
        glm::mat4& getViewMatrix();
        glm::mat4& getProjectionMatrix();

        // Functions
        glm::mat4 lookAt(const glm::vec3 &viewPos = glm::vec3(0.0f, 0.0f, 0.0f));
        glm::vec3 getUp() const;
        glm::vec3 getRight() const;
        glm::vec3 getViewDirection() const;
        /* void setViewDirection(const glm::vec3 &viewDirection) { this->mViewDirection = viewDirection; }; */

        // Movements & rotations
        void move( const glm::vec3 &dist );
        void moveRight( float dist = 1.0f ) { move( glm::vec3(dist,0,0) ); }
        void moveLeft( float dist = 1.0f ) { move( glm::vec3(-dist,0,0) ); }
        void moveUp( float dist = 1.0f ) { move( glm::vec3(0,dist,0) ); }
        void moveDown( float dist = 1.0f ) { move( glm::vec3(0,-dist,0) ); }
        void moveForward( float dist = 1.0f ) { move( glm::vec3(0,0,-dist) ); }
        void moveBack( float dist = 1.0f ) { move( glm::vec3(0,0,dist) ); }

        void rotate( const glm::vec2 &degree );
        void turnRight( float degree ) { rotate( glm::vec2(degree,0) ); }
        void turnLeft( float degree ) { rotate( glm::vec2(-degree,0) ); }
        void turnUp( float degree ) { rotate( glm::vec2(0,degree) ); }
        void turnDown(  float degree ) { rotate( glm::vec2(0,-degree) ); }

        // Callback functions
        void processMouse(float rotH, float rotV);

        // Adrian
        /* void moveCamera(double &xpos, double &ypos, GLFWwindow* window); */
        /* void setDeltaTime(double &time); */

    private:
        glm::vec3   mPos;

        float       mNearPlane;
        float       mFarPlane;
        float       mHFoV;
        float       mAspectRatio;
        float       mHorizontalAngle;
        float       mVerticalAngle;

        /* double      mDeltaTime; */
        float       mMouseSpeed;
        float       mCamSpeed;

};
