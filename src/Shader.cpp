/*
 * readFile.cpp
 *
 *  Created on: Apr 25, 2015
 *      Author: adrian
 */
#include "Shader.h"

using namespace std;

string Shader::readFile(const char *filePath){
	string content;
	ifstream fileStream(filePath, ios::in);
	if(!fileStream.is_open()){
		cerr << "Kann nicht geoeffnet werden. " << filePath << " existiert nicht." << endl;
		return "";
	}
	string line = "";
	while (getline(fileStream, line)){
		content.append(line +"\n");
	}
	fileStream.close();
	return content;
}

bool Shader::shaderSuccess(GLint shader){
	GLint success;
	GLchar infoLog[512];
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if(!success){
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		cerr << "Fehler VertexShader: " << endl << infoLog << endl;
        return false;
	}
    return true;
}

void Shader::printProgramSuccess(GLint program){
	GLint success;
	GLchar infoLog[512];
	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if(!success){
		glGetProgramInfoLog(program, 512, NULL, infoLog);
		cerr << "Fehler ShaderProgram: " << infoLog << endl;
	}
}

GLint Shader::loadShader(const char *shaderPath, ShaderType type)
{
    const GLchar* file  = readFile(shaderPath).c_str();
    GLenum glShaderType;
    switch(type)
    {
        case ShaderType.Fragment                : glShaderType = GL_FRAGMENT_SHADER; break;
        case ShaderType.Vertex                  : glShaderType = GL_VERTEX_SHADER; break;
        case ShaderType.TesselationControl      : glShaderType = GL_TESS_CONTROL_SHADER; break;
        case ShaderType.TesselationEvaluation   : glShaderType = GL_TESS_EVALUATION_SHADER; break;
        case ShaderType.Geometry                : glShaderType = GL_GEOMETRY_SHADER; break;
    }
    GLint shaderRef     = glCreateShader( glShaderType );

	glShaderSource(shaderRef, 1, &file, NULL);
	glCompileShader(shaderRef);

	if(shaderSuccess(shaderRef))
    {
        shaderArray[type] = shaderRef;
    }

    return shaderRef;
}

GLint Shader::linkShader(){

	GLint shaderProgram;
	shaderProgram = glCreateProgram();

    // Attach shaders
    for(GLint shaderElement : shaderArray)
	    if(shader != 0) glAttachShader(shaderProgram, shaderElement);
	
    glLinkProgram(shaderProgram);

    // Delete Shaders
    for(GLint shaderElement : shaderArray)
	    if(shader != 0) glDeleteShader(shaderProgram, shaderElement);

    return shaderProgram;
}


