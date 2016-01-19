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

void Grass::setPositionsFromArray(float* data, int noElements){
	this->fdata = data;
	this->noElements = noElements;
}

void Grass::generatePositionsFromTexture(float* textureData, int width, int height, float minInterval, float maxInterval){
	//fdata = new float[width * height * 3];

	float minPos = -5.0f;
	float posRange = 10.0f;

	float* iterTexture = textureData;
	for (int y = 0; y < height; y++){
		for (int x = 0; x < width; x++)
		{
			if(*iterTexture > 0.2f && *iterTexture < 0.7f){
				float xRatio = static_cast<float>(x) / static_cast<float>(width -1);
				float yRatio = 1.0f - (static_cast<float>(y) / static_cast<float>(height - 1));
				vfdata.push_back(-minPos - (yRatio * posRange));

				vfdata.push_back(*iterTexture);
				vfdata.push_back(minPos + (xRatio * posRange));
			}
			iterTexture++;
		}

	}

}

void Grass::setShaderProgram(GLint shaderProgram){
	this->shaderProgram = shaderProgram;
}

void Grass::draw(){
	glUseProgram(shaderProgram);

	glUniformMatrix4fv(gVPLocation, 1, GL_FALSE, glm::value_ptr(projection * view));
	glUniform3fv(cameraPosLocation, 1, value_ptr(cameraPos));
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(vao);
	glDrawArrays(GL_POINTS, 0, vfdata.size()/3);

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
	glBufferData(GL_ARRAY_BUFFER, vfdata.size() * sizeof(GLfloat), &vfdata.front(), GL_STATIC_DRAW);

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
