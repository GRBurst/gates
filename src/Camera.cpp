#include "Camera.h"
#include <iostream>

Camera::Camera() :
    mPos(glm::vec3(0.0f, 0.0f, 0.0f))
    , mNearPlane(0.1f)
    , mFarPlane(1000.0f)
    , mHFoV(45.0f)
    , mAspectRatio(16.0/9.0f)
    , mHorizontalAngle(3.14f)
    , mVerticalAngle(0.0f)
    /* , mDeltaTime(0.0) */
    , mMouseSpeed(0.001f)
    , mCamSpeed(0.05f)
{
}

// Calculatable matrizes from camera
glm::mat4 Camera::getProjectionMatrix()
{
	return glm::perspective(
            this->mHFoV,
            this->mAspectRatio,
            this->mNearPlane,
            this->mFarPlane
        );
}

glm::mat4 Camera::getViewMatrix()
{
    return glm::lookAt(
            this->mPos,
            this->mPos + getViewDirection(),
            getUp()
        );
}

// Functions
glm::mat4 Camera::lookAt(const glm::vec3 &viewPos)
{
    return glm::lookAt(
            this->mPos,
            viewPos,
            getUp()
        );
}

glm::vec3 Camera::getRight() const
{
    return glm::normalize(glm::vec3(
            sin(this->mHorizontalAngle - 3.14f/2.0f),
            0,
            cos(this->mHorizontalAngle - 3.14f/2.0f)
        ));
}

glm::vec3 Camera::getViewDirection() const
{
    return glm::normalize(glm::vec3(
            cos(this->mVerticalAngle) * sin(this->mHorizontalAngle),
            sin(this->mVerticalAngle),
            cos(this->mVerticalAngle) * cos(this->mHorizontalAngle)
        ));
}

glm::vec3 Camera::getUp() const
{
    return glm::normalize(glm::cross(getRight(), getViewDirection()));
}


void Camera::move( const glm::vec3 &dist )
{
    glm::vec3 vd = getViewDirection();
    std::cout << "mPos: x = " << mPos.x << ", y = " << mPos.y << ", z = " << mPos.z << std::endl;
    this->mPos += getViewDirection() * mCamSpeed * dist;
    std::cout << "viewDirection: x = " << vd.x << ", y = " << vd.y << ", z = " << vd.z << std::endl;
    std::cout << "camera speed = " << mCamSpeed << std::endl;
    std::cout << "dist.x = " << dist.x << ", dist.y = " << dist.y << ", dist.z = " << dist.z << std::endl;
    std::cout << "new mPos: x = " << mPos.x << ", y = " << mPos.y << ", z = " << mPos.z << std::endl;
}

void Camera::rotate( const glm::vec2 &degree )
{
    this->mHorizontalAngle    = fmodf(this->mHorizontalAngle   + glm::radians(degree.x), 314.0f);
    this->mVerticalAngle      = fmodf(this->mVerticalAngle     + glm::radians(degree.y), 314.0f);
}

void Camera::processMouse(float rotH, float rotV)
{
    this->mHorizontalAngle  = fmodf(this->mHorizontalAngle   + (mMouseSpeed * rotH), 314.0f);
    this->mVerticalAngle    = fmodf(this->mVerticalAngle     + (mMouseSpeed * rotV), 314.0f);
    std::cout << "mPos: x = " << mPos.x << ", y = " << mPos.y << ", z = " << mPos.z << std::endl;
    std::cout << "mHorizontalAngle = " << mHorizontalAngle << ", mVerticalAngle = " << mVerticalAngle << std::endl;
}







// Deprecated

/* void Camera::moveCamera(double &xpos, double &ypos, GLFWwindow* window){ */

/* 	int width, height; */

/* 	glfwGetWindowSize(window, &width, &height); */
/* 	glfwSetCursorPos(window, width/2, height/2); */
/* 	mHorizontalAngle += mMouseSpeed * float( width/2 - xpos ); */
/* 	mVerticalAngle   += mMouseSpeed * float( height/2 - ypos ); */
/*     mViewDirection = glm::vec3( */
/* 				cos(mVerticalAngle) * sin(mHorizontalAngle), */
/* 				sin(mVerticalAngle), */
/* 				cos(mVerticalAngle) * cos(mHorizontalAngle) */
/* 				); */
/*     // Right vector */
/* 	mRight = glm::vec3( */
/* 			sin(mHorizontalAngle - 3.14f/2.0f), */
/* 			0, */
/* 			cos(mHorizontalAngle - 3.14f/2.0f) */
/* 	); */
/* 	// Up vector */
/* 	mUp = cross( mRight, mViewDirection ); */
/* 	// Move forward */
/* 	if (glfwGetKey( window, GLFW_KEY_W ) == GLFW_PRESS){ */
/* 			mPos += mViewDirection * (float)mDeltaTime * mCamSpeed; */
/* 	} */
/* 	// Move backward */
/* 	if (glfwGetKey( window, GLFW_KEY_S ) == GLFW_PRESS){ */
/* 			mPos -= mViewDirection * (float)mDeltaTime * mCamSpeed; */
/* 	} */
/* 	// Strafe right */
/* 	if (glfwGetKey( window, GLFW_KEY_D ) == GLFW_PRESS){ */
/* 			mPos += mRight * (float)mDeltaTime * mCamSpeed; */
/* 	} */
/* 	// Strafe left */
/* 	if (glfwGetKey( window, GLFW_KEY_A ) == GLFW_PRESS){ */
/* 			mPos -= mRight * (float)mDeltaTime * mCamSpeed; */
/* 	} */
/* } */

/* void Camera::setDeltaTime(double &time){ */
/* 	mDeltaTime = time; */
/* } */
