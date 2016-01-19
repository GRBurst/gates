/*
 * Grass.cpp
 *
 *  Created on: Jan 17, 2016
 *      Author: adrian
 */

#include "Grass.h"

Grass::Grass()
{
	// TODO Auto-generated constructor stub

}

Grass::~Grass()
{
	// TODO Auto-generated destructor stub
}


void Grass::loadTexture(){
	texture = new Texture("../src/textures/billboardflowers.png");
	texture->linkTexture(shaderProgram, "textureMat");
	texture->bind();
	texture->loadCommonOptions();

}

void Grass::setPositionsFromArray(float* data){
	this->data = data;
}

void Grass::setShaderProgram(GLint shaderProgram){
	this->shaderProgram = shaderProgram;
}

void Grass::draw(){
	GLint err;
	glUseProgram(shaderProgram);

	glUniformMatrix4fv(gVPLocation, 1, GL_FALSE, glm::value_ptr(projection * view));
	glUniform3fv(cameraPosLocation, 1, value_ptr(cameraPos));
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(vao);
	glDrawArrays(GL_POINTS, 0, 4);

	glBindVertexArray(0);

}


void Grass::setBuffers(){
	GLint posAttrib = glGetAttribLocation(shaderProgram, "vPosition");

	//Erzeuge Vertex Array Object und Vertex Buffer Object
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glBindVertexArray(vao);

	//Binde VBO an VAO
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 12 *sizeof(GLfloat), data, GL_STATIC_DRAW);

    glEnableVertexAttribArray(posAttrib);
    glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);


	//Unbind VBO
	//Setze Texture in ShaderProgram an TEXTURE0
	/* if (textureCount >= 1) */
	/* { */
	/* 	texture->setTexture(shaderProgram, 0, "mainTexture"); */
	/* } */

	/* //Setze Texture in ShaderProgram an TEXTURE1 */
	/* if (textureCount >= 2) */
	/* { */
	/* 	normalmap->setTexture(shaderProgram, 1, "normalMap"); */
	/* } */
	/* //glBindBuffer(GL_ARRAY_BUFFER, 0); */
	/* if (textureCount >= 3) */
	/* { */
	/* 	heightmap->setTexture(shaderProgram, 2, "heightMap"); */
	/* } */

	//Unbinde VAO
	glBindVertexArray(0);
}

void Grass::setUniforms(){
    gVPLocation = glGetUniformLocation(shaderProgram, "gVP");
    cameraPosLocation = glGetUniformLocation(shaderProgram, "gCameraPos");
}

void Grass::setViewAndProjectionMatrix(glm::mat4 view, glm::mat4 projection){
	this->view = view;
	this->projection = projection;
}

void Grass::setCameraPosRef(glm::vec3 cameraPos){
	this->cameraPos = cameraPos;
}
