#ifndef LOADSHADER_H_
#define LOADSHADER_H_

#include <iostream>
#include <fstream>
#include <string>
#include <GL/glew.h>
#include <vector>

class Shader
{
    public:
        Shader();
        enum ShaderType
        {
            FRAGMENT = 0,
            VERTEX,
            TESS_CONTROL,
            TESS_EVAL,
            GEOMETRY
        };

        GLint loadShader(const char* shaderPath, ShaderType type);
        GLint linkShaders();


    private:
        bool programSuccess(GLint program);
        bool shaderSuccess(GLint shader);
        bool shaderReady();
        std::string readFile(const char *filePath);

        GLint shaderArray[5] = {0};
};


#endif /* LOADSHADER_H_ */
