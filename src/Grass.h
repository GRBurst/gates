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
#include "Texture.h"

class Grass
{
public:
	Grass();
	~Grass();
	void setShaderProgram(GLint shaderProgram);
	void setUniforms();
	void setBuffers();
	void generatePositionsFromTexture(float* textureData, int width, int height, float minInterval, float maxInterval); //set Interval for values where grass should grow
	void setPositionsFromArray(float* data);
	void setViewAndProjectionMatrix(glm::mat4 view, glm::mat4 projection);
	void draw();
	void setCameraPosRef( glm::vec3 cameraPos);
	void loadTexture();
private:
	GLint shaderProgram;
	GLuint vao, vbo;
	float *data;
	GLint gVPLocation, cameraPosLocation;
	glm::mat4 view;
	glm::mat4 projection;
	glm::vec3 cameraPos;
	Texture *texture;
};

#endif /* SRC_GRASS_H_ */
