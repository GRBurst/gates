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
            Fragment = 0,
            Vertex,
            TesselationControl,
            TesselationEvaluation,
            Geometry
        };

        GLint loadShader(const char* shaderPath, ShaderType type);
        GLint linkShaders();


    private:
        void printProgramSuccess(GLint program);
        void printShaderSuccess(GLint shader);
        string readFile(const char *filePath);

        GLint shaderArray[5] = {0};
};


#endif /* LOADSHADER_H_ */
