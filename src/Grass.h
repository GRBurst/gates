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
	GLint shaderProgram;
	GLuint vao, vbo;
	float *fdata;

	std::vector<GLfloat> vfdata;
	GLint uVPLocation, uCameraPosLocation, uAlphaTest, uAlphaMultiplier;
	glm::mat4 view;
	glm::mat4 projection;
	glm::vec3 cameraPos;
	Texture *texture;
	int mTotalIndices;
	float fAlphaTest;
	float fAlphaMultiplier;
};

#endif /* SRC_GRASS_H_ */
