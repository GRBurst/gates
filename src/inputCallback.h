#ifndef KEYCALLBACK_H
#define KEYCALLBACK_H
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Camera.h"
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
/* void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode, const Camera &camera, float deltaTime); */
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
/* void mouse_callback(GLFWwindow* window, double xpos, double ypos, const Camera &camera); */
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
/* void scroll_callback(GLFWwindow* window, double xoffset, double yoffset, const Camera &camera); */
#endif
