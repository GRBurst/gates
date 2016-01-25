/*
 * Clouds.cpp
 *
 *  Created on: Jan 24, 2016
 *      Author: adrian
 */

#include "Clouds.h"
#include <iostream>
Clouds::Clouds( GLint &shaderProgram, int cloudNumber, float* geometry, Camera* cam) : mCloudNumber(0), mShaderProgram(0),  mVao(0), mVbo(0), muVPLocation(0), muTime(0),
								mGeometry(0), camera(0)
{
	this->mCloudNumber = cloudNumber;
	this->mShaderProgram = shaderProgram;
	this->mGeometry = geometry;
	this->camera = cam;
}

Clouds::~Clouds()
{
	// TODO Auto-generated destructor stub
}


void Clouds::setBuffers(){
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glUseProgram(mShaderProgram);

	GLint posAttrib = glGetAttribLocation(mShaderProgram, "vPosition");
//	GLint uvAttrib = glGetAttribLocation(mShaderProgram, "vUV");
//	//GLint normalAttrib = glGetAttribLocation(mShaderProgram, "vNormal");
	muVPLocation = glGetUniformLocation(mShaderProgram, "uVPMatrix");
	muTime = glGetUniformLocation(mShaderProgram, "uTime");
	glGenVertexArrays(1, &mVao);
	glBindVertexArray(mVao);

	//Generate & bind vbo
	glGenBuffers(1, &mVbo);
	glBindBuffer(GL_ARRAY_BUFFER, mVbo);
	glBufferData(GL_ARRAY_BUFFER, 3 * mCloudNumber * sizeof(GLfloat), mGeometry, GL_STATIC_DRAW);
//	std::cout <<"HIER";
//	std::cout << mCloudNumber;
//	for(int i = 0; i < mCloudNumber * 3; i++){
//		std::cout << mGeometry[i] << std::endl;
//		std::cout.flush();
//	}
	// Get Attrib location
	/* glEnableVertexAttribArray(terrainPosAttrib); */
	/* glVertexAttribPointer(terrainPosAttrib, 3, GL_FLOAT, GL_FALSE, 24, 0); */
	/* glEnableVertexAttribArray(terrainNormalAttrib); */
	/* glVertexAttribPointer(terrainNormalAttrib, 3, GL_FLOAT, GL_FALSE, 24, reinterpret_cast<const GLvoid*>(12)); */

	// First all positions, then all normals
	glEnableVertexAttribArray(posAttrib);
	glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
	glBindVertexArray(0);

}

void Clouds::draw(){
	glUseProgram(mShaderProgram);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glUniformMatrix4fv(muVPLocation, 1, GL_FALSE, glm::value_ptr(camera->getVPMatrix()));
	glUniform1f(muTime, glfwGetTime());
	glBindVertexArray(mVao);
	glDrawArrays(GL_POINTS, 0, mCloudNumber * 3);
	glDisable(GL_BLEND);
	glBindVertexArray(0);
}

void Clouds::loadTexture(float* textureData, int width, int height, int depth){
	Texture* texture = new Texture();
	texture->setData(textureData, width, height, depth);
	texture->linkTexture(mShaderProgram, "tClouds");
	texture->bind3D();
	texture->loadSkydome3DOptions();
}
