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
        void setDim(int dim) {mDim = dim;};
        void render();
        void render(Texture& texture);
        void render(const GLint& shader, Texture& texture);

    private:
        GLuint quadVAO;
        GLuint quadVBO;
        GLint mQuadProgram;
        bool mFirstCall;
        int mDim;
};
#endif /* QUAD_H_ */
