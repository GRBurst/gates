#include "Camera.h"
#include <iostream>

Camera::Camera() :
    mPos(glm::vec3(0.0f, 1.0f, 0.0f))
    , mNearPlane(0.1f)
    , mFarPlane(1000.0f)
    , mHFoV(45.0f)
    , mAspectRatio(16.0/9.0f)
    , mHorizontalAngle(3.14f)
    /* , mHorizontalAngle(3.14f) */
    , mVerticalAngle(0.0f)
    /* , mDeltaTime(0.0) */
    , mMouseSpeed(0.001f)
    , mCamSpeed(1.0f)
{
}


// Calculatable matrizes from camera
void Camera::calcProjectionMatrix()
{
	mProjectionMatrix = glm::perspective(
            this->mHFoV,
            this->mAspectRatio,
            this->mNearPlane,
            this->mFarPlane
        );
}

void Camera::calcViewMatrix()
{
    mViewMatrix = glm::lookAt(
            this->mPos,
            this->mPos + getViewDirection(),
            getUp()
        );
}

void Camera::update()
{
    calcViewMatrix();
    calcProjectionMatrix();
    mViewProjectionMatrix = mProjectionMatrix * mViewMatrix;
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
    this->mPos += mCamSpeed * dist;
}

void Camera::rotate( const glm::vec2 &degree )
{
    this->mHorizontalAngle    = fmodf(this->mHorizontalAngle   + glm::radians(degree.x), 314.0f);
    this->mVerticalAngle      = fmodf(this->mVerticalAngle     + glm::radians(degree.y), 314.0f);
}

void Camera::processMouse(float rotH, float rotV)
{
    /* this->mHorizontalAngle  = fmodf(this->mHorizontalAngle   + (mMouseSpeed * rotH), 314.0f); */
    /* this->mVerticalAngle    = fmodf(this->mVerticalAngle     + (mMouseSpeed * rotV), 314.0f); */
    this->mHorizontalAngle  = this->mHorizontalAngle   + (mMouseSpeed * rotH);
    this->mVerticalAngle    = this->mVerticalAngle     + (mMouseSpeed * rotV);
}
