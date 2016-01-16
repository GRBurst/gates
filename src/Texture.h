#ifndef TEXTURE_H_
#define TEXTURE_H_

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <SOIL/SOIL.h>
#include "GLerror.h"

class Texture
{

    public:
        Texture();
        ~Texture() {};

        GLuint getTexture();
        GLuint getTextureLoc();

        void setTexture(GLint shaderProgram, GLint location, const char *texture_name);
        void setData(unsigned char& data);
        void loadCommonOptions();
        void bind();
        void unbind();


    private:
        GLuint texture;
        GLuint textureLocation;
        GLint location;

        int width, height;
        unsigned char* data;

}
#endif /* TEXTURE_H_ */
