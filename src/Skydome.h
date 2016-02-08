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
#include <random>
#include <cmath>
#include <algorithm>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Camera.h"
#include "Texture.h"
#include "Clouds.h"
#include <GLFW/glfw3.h>

class Skydome
{
public:
	Skydome(GLint shaderProgram, Camera* cam);
	virtual ~Skydome();
	void draw();
	void drawReflection();
	void generateGeometry(float r, int azimuths, int meridians);
	void setBuffers();
	void setCloud(GLint cloudShader, double probabilty);
	void loadTexture(float* textureData, int width, int height, int depth);
	void loadTexture(float* textureData, int width, int height);
	void setClouds(Clouds* clouds){mClouds = clouds;}
	float* getCloudAttributes(){return &cloudAttributes.front()[0];}
	int getCloudNumber(){return cloudNumber; }
private:
	//bool sortZ (glm::vec3 i, glm::vec3 j);
	Clouds *mClouds;
	std::vector<float> meshAttributes;
	std::vector<glm::vec3> cloudAttributes;
	GLuint mVao;
	GLuint mVbo;
	GLint mShaderProgram;
	GLuint muVPLocation;
	GLuint muInvViewLocation;
	GLuint muTime, muRadius;
	Camera* camera;
	float mRadius;
	int verticesNumber, cloudNumber;
	int mCloudSeed;
	double mCloudProbability;

};

#endif /* SRC_SKYDOME_H_ */
