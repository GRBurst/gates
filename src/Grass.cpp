/*
 * Grass.cpp
 *
 *  Created on: Jan 17, 2016
 *      Author: adrian
 */

#include "Grass.h"

Grass::Grass() : shaderProgram(0), vao(0), vbo(0),
				fdata(0), vfdata(std::vector<GLfloat>(0)), uVPLocation(0),
				uCameraPosLocation(0), view(glm::mat4(0)), projection(glm::mat4(0)),
				cameraPos(glm::vec3(0)), texture(0), mTotalIndices(0)
{
	// TODO Auto-generated constructor stub

}

Grass::~Grass()
{
	// TODO Auto-generated destructor stub
}


void Grass::loadTexture(){
	texture = new Texture("../src/textures/grass.png");
	texture->linkTexture(shaderProgram, "textureMat");
	texture->bind();
	texture->loadCommonOptions();

}

void Grass::setTerrainVao(GLuint vao, int totalIndices){
	this->vao = vao;
	this->mTotalIndices = totalIndices;
}
//Deprecated__ only for debug purpose
//void Grass::setPositionsFromArray(float* data, int noElements){
//	this->fdata = data;
//	this->mIndices = noElements;
//}

//void Grass::generatePositionsFromTexture(float* textureData, int width, int height, float minInterval, float maxInterval){
//	//fdata = new float[width * height * 3];
//
//	float minPos = -5.0f;
//	float posRange = 10.0f;
//
//	float* iterTexture = textureData;
//	for (int y = 0; y < height; y++){
//		for (int x = 0; x < width; x++)
//		{
//			if(*iterTexture > 0.2f && *iterTexture < 0.7f){
//				float xRatio = static_cast<float>(x) / static_cast<float>(width -1);
//				float yRatio = 1.0f - (static_cast<float>(y) / static_cast<float>(height - 1));
//				vfdata.push_back(minPos + (xRatio * posRange));
//				vfdata.push_back(*iterTexture);
//				vfdata.push_back(-minPos - (yRatio * posRange));
//			}
//			iterTexture++;
//		}
//
//	}
//
//}
//////////////////////////////////////

void Grass::setShaderProgram(GLint shaderProgram){
	this->shaderProgram = shaderProgram;
}


void Grass::draw(){
	float time = glfwGetTime();

	glUseProgram(shaderProgram);
//	glEnable(GL_BLEND);
//	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	GLenum err = GL_NO_ERROR;
	glUniformMatrix4fv(uVPLocation, 1, GL_FALSE, glm::value_ptr(projection * view));
	glUniform3fv(uCameraPosLocation, 1, value_ptr(cameraPos));
	glUniform1f(uAlphaTest, 0.25f);
	glUniform1f(uAlphaMultiplier, 1.5f);
	glUniform1f(uTime, time);

    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(vao);
    //glDisable(GL_ALPHA_TEST);

    glDrawElements(GL_TRIANGLE_STRIP, mTotalIndices, GL_UNSIGNED_INT, 0);
	//glDrawArrays(GL_POINTS, 0, vfdata.size()/3);

       err = glGetError();
   	if (err != GL_NO_ERROR)
   		std::cout << "Fehler 79(): " << err << std::endl;
   	std::cout.flush();
	glBindVertexArray(0);

	//glEnable(GL_CULL_FACE);
}


void Grass::setBuffers(){
	GLint posAttrib = glGetAttribLocation(shaderProgram, "vPosition");
	//Bind Vertex Array of Terrain
	glBindVertexArray(vao);

    glEnableVertexAttribArray(posAttrib);
    glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);


	//Unbinde VAO
	glBindVertexArray(0);
}

void Grass::setUniforms(){
    uVPLocation = glGetUniformLocation(shaderProgram, "gVP");
    uCameraPosLocation = glGetUniformLocation(shaderProgram, "gCameraPos");
    uAlphaTest = glGetUniformLocation(shaderProgram, "fAlphaTest");
    uAlphaMultiplier = glGetUniformLocation(shaderProgram, "fAlphaMultiplier");
    uTime = glGetUniformLocation(shaderProgram, "uTime");
}

void Grass::setViewAndProjectionMatrix(glm::mat4 view, glm::mat4 projection){
	this->view = view;
	this->projection = projection;
}

void Grass::setCameraPosRef(glm::vec3 cameraPos){
	this->cameraPos = cameraPos;
}
