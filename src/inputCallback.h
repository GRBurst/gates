#ifndef KEYCALLBACK_H
#define KEYCALLBACK_H
#include <GL/glew.h>
#include <GLFW/glfw3.h>
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
#endif
