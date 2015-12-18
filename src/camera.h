#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera
{

    public:
	    Camera();
	    //void setPosition(const glm::vec3 &position);
        glm::vec3 getPosition();
        glm::mat4 lookAt(   const glm::vec3 &camPos = glm::vec3(0.0f, 0.0f, 0.0f)
                        , const glm::vec3 &viewDirection = glm::vec3(0.0f, 0.0f, -1.0f)
                        , const glm::vec3 &up = glm::vec3(0.0f, 1.0f, 0.0f)
                        );

    private:
	    void buildFrustum(float fov, float ratio, float near, float far);

        glm::vec3   camPos;
        glm::mat4   frustum;

};
