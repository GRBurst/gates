#ifndef QUAD_H_
#define QUAD_H_
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>
#include "Camera.h"
#include "Shader.h"
#include "Texture.h"

/*
 * Usage:
 * 1.) Construct
 * 2.) Render
 */
class Quad {

    public:
        Quad();
        ~Quad();

        void init();
        void render();
        void render(Texture& texture);
        void render(const GLint& shader, Texture& texture);

    private:
        GLuint quadVAO = 0;
        GLuint quadVBO;
        GLint mQuadProgram;
        bool mFirstCall = 1;
};
#endif /* QUAD_H_ */
