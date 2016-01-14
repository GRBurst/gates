#ifndef MODELLOADER_H_
#define MODELLOADER_H_

#include <vector>
#include <array>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <fstream>
#include <SOIL/SOIL.h>
/* #include "Texture.h" */

using namespace std;
using namespace glm;

class ModelLoader
{
friend class GameObject;
public:
	ModelLoader(const char* filePath, GLint shaderProgram);
	/* ModelLoader(const char* filePath, const char* imagePath, GLint shaderProgram); */
	/* ModelLoader(const char* filePath, const char* imagePath, const char* normalMapPath, GLint shaderProgram); */
	/* ModelLoader(const char* filePath, const char* imagePath, const char* normalMapPath, const char* heightMapPath, GLint shaderProgram); */
	~ModelLoader();
	
    void loadFile();
	void setStandardUniforms();
	void setBuffers();
	void setStandardUniformLocations();
	void setStandardTexAttribLocations();
	void draw();
	void draw(GLuint &locations);
	void setProjection(mat4 &projection);
	void setView(mat4 &view);
	void setMVP();
	void scale(vec3 &scale);
private:
	GLuint VBO, VAO, modelLocation, projectionLocation, viewLocation, MVPLocation, MVLocation, MV3Location, lightLoc, cameraLoc;
	GLint shaderProgram;
	vector<vec3> fileVertices;
	vector<vec3> vertices;
	vector<vec2> fileUvs;
	vector<vec2> uvs;
	vector<vec3> fileNormals;
	vector<vec3> normals;
	vector<vec3> tangents;
	vector<vec3> bitangents;
	vec3 lightPosition;
	vec3 cameraPosition;
	vector<int> vertexIndices, normalIndices, uvIndices;
	mat4 view = lookAt(vec3(0.0f, 0.0f, 5.0f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));
	mat4 projection;
	mat4 model = mat4(1.0f);
	mat4 MVP, MV;
	mat3 MV3;
	mat4 invMV;
	ifstream *fileStream;
	GLfloat *vertexBuffer;
	/* Texture *texture; */
	/* Texture *normalmap; */
	/* Texture *heightmap; */
	void setVerticesAsArray();
	void setStride();

	void calculateTangents();
	vector<vec3> getVertices();
	vector<vec2> getUVs();
	vector<vec3> getNormals();
	vector<int> getVertexIndices(), getNormalIndices(), getUVIndices();

	void changeShader(GLint &shaderProgram);
	/* void loadTexture(); */
	GLenum err = GL_NO_ERROR;

	void translate(vec3 &trans);
	void rotateAroundSelf(float &angle, vec3 &axis);

	void rotateAroundAxis(float &angle, vec3 &axis);
	void backToCenter();
	/* int vertexCount, normalCount, uvCount, faces, textureCount; */
	int vertexCount, normalCount, uvCount, faces;
};



#endif /* MODELLOADER_H_ */
