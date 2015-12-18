#include "camera.h"

void Camera::buildFrustum(float fov, float ratio, float near, float far)
{
    frustum = glm::perspective(fov, ratio, near, far);
}

glm::vec3 Camera::getPosition()
{
    return camPos;
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
