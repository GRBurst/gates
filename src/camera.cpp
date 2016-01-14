#include "camera.h"

Camera::Camera() :
    mCamPos(glm::vec3(0.0f, 0.0f, 0.0f))
    , mViewDirection(glm::vec3(0.0f, 0.0f, -1.0f))
    , mUp(glm::vec3(0.0f, 1.0f, 0.0f))
    , mRight(glm::vec3(1.0f, 0.0f, 0.0f))
    /* , mFrustum() */
    , mViewMatrix()
    , mProjectionMatrix()
    , mNearPlane(0.1f)
    , mFarPlane(1000.0f)
    , mFieldOfViewHorizontal(45.0f)
    , mAspectRatio(16.0/9.0f)
    , mHorizontalAngle(3.14f)
    , mVerticalAngle(0.0f)
    , mDeltaTime(0.0)
    , mMouseSpeed(0.0005f)
    , mCamSpeed(3.0f)
{
}

// Getters & Setters
glm::vec3 Camera::getCamPos()
{
    return mCamPos;
}
void Camera::setCamPos(const glm::vec3 &position)
{
    mCamPos = position;
}


glm::vec3 Camera::getViewDirection()
{
    return mViewDirection;
}
void Camera::setViewDirection(const glm::vec3 &viewDirection)
{
    mViewDirection = viewDirection;
}

float Camera::getNearPlane()
{
    return mNearPlane;
}
void Camera::setNearPlane(float nearPlane)
{
    mNearPlane = nearPlane;
}

float Camera::getFarPlane()
{
    return mFarPlane;
}
void Camera::setFarPlane(float farPlane)
{
    mFarPlane = farPlane;
}

float Camera::getFielOfViewHorizontal()
{
    return mFieldOfViewHorizontal;
}
void Camera::setFielOfViewHorizontal(float fieldOfViewHorizontal)
{
    mFieldOfViewHorizontal = fieldOfViewHorizontal;
}

float Camera::getAspectRatio()
{
    return mAspectRatio;
}
void Camera::setAspectRatio(float aspectRatio)
{
    mAspectRatio = aspectRatio;
}

/* glm::mat4 Camera::getFrustum() */
/* { */
/*     return mFrustum; */
/* } */
/* void Camera::setFrustum(glm::mat4 frustum) */
/* { */
/*     mFrustum = frustum; */
/* } */

/* glm::mat4 Camera::getViewMatrix() */
/* { */
/*     return mViewMatrix; */
/* } */
void Camera::setViewMatrix(glm::mat4 viewMatrix)
{
    mViewMatrix = viewMatrix;
}

glm::mat4& Camera::getProjectionMatrix()
{
	 mProjectionMatrix = glm::perspective(mFieldOfViewHorizontal, mAspectRatio, mNearPlane, mFarPlane);
    return mProjectionMatrix;
}
void Camera::setProjectionMatrix(glm::mat4 projectionMatrix)
{
    mProjectionMatrix = projectionMatrix;
}

// Functions
/* void Camera::buildFrustum(float fov, float ratio, float near, float far) */
/* { */
/*     mFrustum = glm::perspective(fov, ratio, near, far); */
/* } */

glm::mat4 Camera::lookAt(const glm::vec3 &camPos, const glm::vec3 &viewDirection, const glm::vec3 &up)
{
    mCamPos = camPos;
    mViewDirection = viewDirection;

    glm::mat4 view = lookAt(
                camPos,
                camPos+viewDirection,
                up
            );
	return view;
}

/* glm::vec3 Camera::getUpDirection     () const */
/* { */
/*     glm::vec3 up(); */
/*     return up; */
/* } */
/* glm::vec3 Camera::getRightDirection  () const */
/* { */
/*     glm::vec3 right(); */
/*     return right; */
/* } */
/* glm::vec3 Camera::getForwardDirection() const */
/* { */
/*     glm::vec3 forward(); */
/*     return forward; */
/* } */

// Adrian
glm::mat4& Camera::getViewMatrix(){
	mViewMatrix = glm::lookAt(
            mCamPos,           // Camera is here
            mCamPos+mViewDirection, // and looks here : at the same position, plus "direction"
            mUp                  // Head is up (set to 0,-1,0 to look upside-down)
			);
//	std::cout << "view: " << mViewMatrix[0][0] << "< " << mViewMatrix[0][1] << "< " << mViewMatrix[0][2] << std::endl;
	        //processInput
	return mViewMatrix;
}

void Camera::moveCamera(double &xpos, double &ypos, GLFWwindow* window){

	int width, height;

	glfwGetWindowSize(window, &width, &height);
	glfwSetCursorPos(window, width/2, height/2);
	mHorizontalAngle += mMouseSpeed * float( width/2 - xpos );
	mVerticalAngle   += mMouseSpeed * float( height/2 - ypos );
    mViewDirection = glm::vec3(
				cos(mVerticalAngle) * sin(mHorizontalAngle),
				sin(mVerticalAngle),
				cos(mVerticalAngle) * cos(mHorizontalAngle)
				);
    // Right vector
	mRight = glm::vec3(
			sin(mHorizontalAngle - 3.14f/2.0f),
			0,
			cos(mHorizontalAngle - 3.14f/2.0f)
	);
	// Up vector
	mUp = cross( mRight, mViewDirection );
	// Move forward
	if (glfwGetKey( window, GLFW_KEY_W ) == GLFW_PRESS){
			mCamPos += mViewDirection * (float)mDeltaTime * mCamSpeed;
	}
	// Move backward
	if (glfwGetKey( window, GLFW_KEY_S ) == GLFW_PRESS){
			mCamPos -= mViewDirection * (float)mDeltaTime * mCamSpeed;
	}
	// Strafe right
	if (glfwGetKey( window, GLFW_KEY_D ) == GLFW_PRESS){
			mCamPos += mRight * (float)mDeltaTime * mCamSpeed;
	}
	// Strafe left
	if (glfwGetKey( window, GLFW_KEY_A ) == GLFW_PRESS){
			mCamPos -= mRight * (float)mDeltaTime * mCamSpeed;
	}
}

void Camera::setDeltaTime(double &time){
	mDeltaTime = time;
}
