#include "inputCallback.h"

/* void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode, const Camera &camera, float deltaTime) */
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    switch(key)
    {
        case GLFW_KEY_ESCAPE :
            if(action == GLFW_PRESS) glfwSetWindowShouldClose(window, GL_TRUE);
            break;
        case GLFW_KEY_Q :
            if(action == GLFW_PRESS) glfwSetWindowShouldClose(window, GL_TRUE);
            break;
        case GLFW_KEY_W :
            if(action == GLFW_PRESS) camera.moveForward( float(deltaTime) );
            break;
        case GLFW_KEY_S :
            if(action == GLFW_PRESS) camera.moveBack( float(deltaTime) );
            break;
        case GLFW_KEY_A :
            if(action == GLFW_PRESS) camera.moveLeft( float(deltaTime) );
            break;
        case GLFW_KEY_D :
            if(action == GLFW_PRESS) camera.moveRight( float(deltaTime) );
            break;
        default: std::cout << "Key has no function!" << std::endl;
            break;
    }
        //mRight * (float)mDeltaTime * mCamSpeed
}

/* void mouse_callback(GLFWwindow* window, double xpos, double ypos, const Camera &camera) */
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    int width, height;

    glfwGetWindowSize(window, &width, &height);
    glfwSetCursorPos(window, width/2, height/2);

    camera.processMouse(float( width/2 - xpos ), float( height/2 - ypos ));

}

/* void scroll_callback(GLFWwindow* window, double xoffset, double yoffset, const Camera &camera) */
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    // TODO
}
