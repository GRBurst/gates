/*
 * Grass.h
 *
 *  Created on: Jan 17, 2016
 *      Author: adrian
 */

#ifndef SRC_GRASS_H_
#define SRC_GRASS_H_
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <random>
#include <GLFW/glfw3.h>
#include "Texture.h"

class Grass
{
public:
	Grass();
	~Grass();
	void setShaderProgram(GLint shaderProgram);
	void setUniforms();
	void setBuffers();
//	void generatePositionsFromTexture(float* textureData, int width, int height, float minInterval, float maxInterval); //set Interval for values where grass should grow
//	void setPositionsFromArray(float* data, int noElements);
	void setViewAndProjectionMatrix(glm::mat4 view, glm::mat4 projection);
	void draw();
	void setCameraPosRef( glm::vec3 cameraPos);
	void loadTexture();
	void setTerrainVao(GLuint vao, int totalIndices);
private:


	Texture *texture;

	glm::mat4 view;
	glm::mat4 projection;
	glm::vec3 cameraPos;
	std::default_random_engine generator;
	std::vector<GLfloat> vfdata;
	GLint shaderProgram;
	GLint uVPLocation, uCameraPosLocation, uAlphaTest, uAlphaMultiplier, uColor, uTime;
	GLuint vao, vbo;

	float *fdata;
	int mTotalIndices, randomSeed;
};

#endif /* SRC_GRASS_H_ */
