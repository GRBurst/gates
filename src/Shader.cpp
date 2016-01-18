#include "Shader.h"

using namespace std;

Shader::Shader(){

}

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

bool Shader::shaderSuccess(GLint shader)
{
	GLint success;
	GLchar infoLog[512];
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if(!success){
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		cerr << "Fehler: " << shader << endl << infoLog << endl;
        return false;
	}
    return true;
}

bool Shader::programSuccess(GLint program){
	GLint success;
	GLchar infoLog[512];
	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if(!success){
		glGetProgramInfoLog(program, 512, NULL, infoLog);
		cerr << "Fehler ShaderProgram: " << infoLog << endl;
        return false;
	}
    return true;
}

GLint Shader::loadShader(const char *shaderPath, ShaderType type)
{
    string tmpFile = readFile(shaderPath);

    const GLchar* file  = tmpFile.c_str();
    GLenum glShaderType = 0;

   // cout << "shader filedata: " << endl << readFile(shaderPath) << endl;

    switch(type)
    {
        case FRAGMENT       : glShaderType = GL_FRAGMENT_SHADER; printf("Detected FRAGMENT shader!\n"); break;
        case VERTEX         : glShaderType = GL_VERTEX_SHADER; printf("Detected VERTEX shader!\n"); break;
        case TESS_CONTROL   : glShaderType = GL_TESS_CONTROL_SHADER; printf("Detected TESS_CONTROL shader!\n"); break;
        case TESS_EVAL      : glShaderType = GL_TESS_EVALUATION_SHADER; printf("Detected TESS_EVAL shader!\n"); break;
        case GEOMETRY       : glShaderType = GL_GEOMETRY_SHADER; printf("Detected GEOMETRY shader!\n"); break;
    }

    if(glShaderType == 0)
        return 0;

    GLint shaderRef = glCreateShader( glShaderType );
	glShaderSource(shaderRef, 1, &file, NULL);
	glCompileShader(shaderRef);

	if(shaderSuccess(shaderRef))
    {
        shaderArray[type] = shaderRef;
    }

    return shaderRef;
}

bool Shader::shaderReady()
{
    for(GLint shaderElement : shaderArray)
	    if(shaderElement != 0) return true;

    return false;
}

GLint Shader::linkShaders(){

    if(!shaderReady())
    {
        cerr << "Can not link shader(s). None loaded yet!" << endl;
        return 0;
    }

	GLint shaderProgram;
    shaderProgram = glCreateProgram();

    // Attach shaders
    for(GLint shaderElement : shaderArray)
	    if(shaderElement != 0) glAttachShader(shaderProgram, shaderElement);
	
    glLinkProgram(shaderProgram);

    programSuccess(shaderProgram);

    // Delete Shaders
    for(GLint shaderElement : shaderArray)
	    if(shaderElement != 0) glDeleteShader(shaderElement);

    return shaderProgram;
}


