#include "camera.h"

Camera::Camera()
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

glm::mat4 Camera::getFrustum()
{
    return mFrustum;
}
void Camera::setFrustum(glm::mat4 frustum)
{
    mFrustum = frustum;
}

glm::mat4 Camera::getViewMatrix()
{
    return mViewMatrix;
}
void Camera::setViewMatrix(glm::mat4 viewMatrix)
{
    mViewMatrix = viewMatrix;
}

glm::mat4 Camera::getProjectionMatrix()
{
    return mProjectionMatrix;
}
void Camera::setProjectionMatrix(glm::mat4 projectionMatrix)
{
    mProjectionMatrix = projectionMatrix;
}

// Functions
void Camera::buildFrustum(float fov, float ratio, float near, float far)
{
    mFrustum = glm::perspective(fov, ratio, near, far);
}

glm::mat4 Camera::lookAt(const glm::vec3 &camPos, const glm::vec3 &viewDirection, const glm::vec3 &up)
{
    glm::mat4 view = lookAt(
                camPos,
                camPos+viewDirection,
                up
            );
	return view;
}
