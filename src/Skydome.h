/*
 * Skydome.h
 *
 *  Created on: Jan 22, 2016
 *      Author: adrian
 */

#ifndef SRC_SKYDOME_H_
#define SRC_SKYDOME_H_

#include <GL/glew.h>
#include <vector>
#include <iostream>
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Camera.h"
#include "Texture.h"

class Skydome
{
public:
	Skydome(GLint shaderProgram, Camera* cam);
	virtual ~Skydome();
	void draw();
	void generateGeometry(float r, int azimuths, int meridians);
	void setBuffers();
	void loadTexture(float* textureData, int width, int height);
private:
	std::vector<float> meshAttributes;
	GLuint mVao;
	GLuint mVbo;
	GLint mShaderProgram;
	GLuint muVPLocation;
	GLuint muInvViewLocation;
	Camera* camera;
	int verticesNumber;
};

#endif /* SRC_SKYDOME_H_ */
