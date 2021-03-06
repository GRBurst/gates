/*
 * Skydome.cpp
 *
 *  Created on: Jan 22, 2016
 *      Author: adrian
 */

#include "Skydome.h"

Skydome::Skydome(GLint shaderProgram, Camera* camera) : mClouds(0), meshAttributes(0), cloudAttributes(0), mVao(0), mVbo(0), mShaderProgram(0),
														muVPLocation(0), muInvViewLocation(0), muTime(0), muRadius(0),
														camera(0), mRadius(10), verticesNumber(0), cloudNumber(0), mCloudSeed(212), mCloudProbability(0.35)
{
	// TODO Auto-generated constructor stub
	mShaderProgram = shaderProgram;
	this->camera = camera;
}

Skydome::~Skydome()
{
	// TODO Auto-generated destructor stub
}

bool sortZ (glm::vec3 i, glm::vec3 j) { return (i.z<j.z); }

void Skydome::loadTexture(float* textureData, int width, int height, int depth){
	Texture* texture = new Texture();
	texture->setData(textureData, width, height, depth);
	texture->linkTexture(mShaderProgram, "textureSky");
	texture->bind3D();
	texture->loadSkydome3DOptions();
}

void Skydome::loadTexture(float* textureData, int width, int height){
	Texture* texture = new Texture();
	texture->setData(textureData, width, height);
	texture->linkTexture(mShaderProgram, "textureSky");
	texture->bind();
	texture->loadSkydomeOptions();
}
void Skydome::drawReflection()
{
	glm::mat4 mMMatrix = glm::mat4(1.0f, 0.0f, 0.0f, 0.0f,
								0.0f, -1.0f, 0.0f, 0.0f,
								0.0f, 0.0f, 1.0f, 0.0f,
								0.0f, 0.0f, 0.0f, 1.0f);
	glUseProgram(mShaderProgram);
	glUniformMatrix4fv(muVPLocation, 1, GL_FALSE, glm::value_ptr(camera->getVPMatrix()));
	glUniform1f(muTime, glfwGetTime());
	glUniform1f(muRadius, mRadius);
	glUniformMatrix4fv(glGetUniformLocation(mShaderProgram, "uMMatrix"), 1, GL_FALSE, glm::value_ptr(mMMatrix));
	glBindVertexArray(mVao);
	glDrawArrays(GL_TRIANGLES, 0, verticesNumber );
	glBindVertexArray(0);

}
void Skydome::draw()
{
	glm::mat4 mMMatrix = glm::mat4(1.0f);
	glUseProgram(mShaderProgram);
	glUniformMatrix4fv(muVPLocation, 1, GL_FALSE, glm::value_ptr(camera->getVPMatrix()));
	glUniform1f(muTime, glfwGetTime());
	glUniform1f(muRadius, mRadius);
	glUniformMatrix4fv(glGetUniformLocation(mShaderProgram, "uMMatrix"), 1, GL_FALSE, glm::value_ptr(mMMatrix));
	glBindVertexArray(mVao);
	glDrawArrays(GL_TRIANGLES, 0, verticesNumber );
	glBindVertexArray(0);
//	mClouds->draw();
}

void Skydome::generateGeometry(float r, int azimuths, int meridians)
{
	float cloudHeightMin = 0.5f;
	float cloudHeightMax = 0.75f * r;
	//UV Generation is spherical
	mRadius = r;
	std::default_random_engine generator(mCloudSeed);
	std::uniform_real_distribution<float> distribution (0.0, r / 5);
	//meshAttributes.resize(azimuths * 30 + (meridians + 1) * azimuths * 30); //azimuths * 2 * 3 * ( 3+ 2) + (meridians - 2) * azimuths * 6 * (3 + 2)
	//verticesNumber = azimuths * 18 + (meridians + 1) * azimuths * 18;
	std::bernoulli_distribution bernoulli(mCloudProbability);
	for( int t = 0 ; t < meridians ; t++ )
	{
		float u1 = (float)(t)/meridians;
		float u2 = (float)(t+1)/meridians;
		float theta1 = u1 * glm::pi<float>();
		float theta2 = u2 * glm::pi<float>();
//		float u1 = std::asin((2*t)/meridians - 1)/glm::pi<float>() + 0.5f;
//		float u2 = std::asin((2*(t + 1))/meridians - 1)/glm::pi<float>() + 0.5f;
//		float theta1 = (float)(t)/meridians * glm::pi<float>();
//		float theta2 = (float)(t+1)/meridians * glm::pi<float>();

		for( int p = 0 ; p < azimuths ; p++ )
		{
			float v1 =  (float)(p)/azimuths;
			float v2 = (float)(p+1)/azimuths;
			float phi1 = v1 * 2 * glm::pi<float>();
			float phi2 = v2 * 2 * glm::pi<float>();
//			float v1 = std::asin((2*p)/azimuths - 1)/glm::pi<float>() + 0.5f;;
//			float v2 = std::asin((2*(p + 1))/azimuths - 1)/glm::pi<float>() + 0.5f;
//			float phi1 = (float)(p)/azimuths * 2 * glm::pi<float>();
//			float phi2 = (float)(p+1)/azimuths * 2 * glm::pi<float>();
			float y = r * cos(theta1);
			glm::vec3 vertex1 = glm::vec3(r * sin(theta1) * cos(phi1), y, r * sin(theta1) * sin(phi1)); //z and y switch to have caps of sphere top and bottom
			glm::vec3 vertex2 = glm::vec3(r * sin(theta1) * cos(phi2), r * cos(theta1), r * sin(theta1) * sin(phi2));
			glm::vec3 vertex3 = glm::vec3(r * sin(theta2) * cos(phi2), r * cos(theta2), r * sin(theta2) * sin(phi2));
			glm::vec3 vertex4 = glm::vec3(r * sin(theta2) * cos(phi1), r * cos(theta2), r * sin(theta2) * sin(phi1));

			if (y > cloudHeightMin && y < cloudHeightMax){
				if(bernoulli(generator)){
					glm::vec3 vertex = vertex1 - (glm::normalize(vertex1) * distribution(generator));
					cloudAttributes.push_back(vertex);
				}
			}

			if( t == 0 )
			{// top cap
				meshAttributes.push_back(vertex1.x);
				meshAttributes.push_back(vertex1.y);
				meshAttributes.push_back(vertex1.z);
				meshAttributes.push_back(u1);
				meshAttributes.push_back(v1);
				meshAttributes.push_back(vertex4.x);
				meshAttributes.push_back(vertex4.y);
				meshAttributes.push_back(vertex4.z);
				meshAttributes.push_back(u2);
				meshAttributes.push_back(v1);
				meshAttributes.push_back(vertex3.x);
				meshAttributes.push_back(vertex3.y);
				meshAttributes.push_back(vertex3.z);
				meshAttributes.push_back(u2);
				meshAttributes.push_back(v2);
			}
			else
			{
				if( t + 1 == meridians ) //end cap
				{
					meshAttributes.push_back(vertex3.x);
					meshAttributes.push_back(vertex3.y);
					meshAttributes.push_back(vertex3.z);
					meshAttributes.push_back(u2);
					meshAttributes.push_back(v2);
					meshAttributes.push_back(vertex2.x);
					meshAttributes.push_back(vertex2.y);
					meshAttributes.push_back(vertex2.z);
					meshAttributes.push_back(u1);
					meshAttributes.push_back(v2);
					meshAttributes.push_back(vertex1.x);
					meshAttributes.push_back(vertex1.y);
					meshAttributes.push_back(vertex1.z);
					meshAttributes.push_back(u1);
					meshAttributes.push_back(v1);
				}
				else
				{

					meshAttributes.push_back(vertex1.x);
					meshAttributes.push_back(vertex1.y);
					meshAttributes.push_back(vertex1.z);
					meshAttributes.push_back(u1);
					meshAttributes.push_back(v1);
					meshAttributes.push_back(vertex4.x);
					meshAttributes.push_back(vertex4.y);
					meshAttributes.push_back(vertex4.z);
					meshAttributes.push_back(u2);
					meshAttributes.push_back(v1);
					meshAttributes.push_back(vertex2.x);
					meshAttributes.push_back(vertex2.y);
					meshAttributes.push_back(vertex2.z);
					meshAttributes.push_back(u1);
					meshAttributes.push_back(v2);
					meshAttributes.push_back(vertex2.x);
					meshAttributes.push_back(vertex2.y);
					meshAttributes.push_back(vertex2.z);
					meshAttributes.push_back(u1);
					meshAttributes.push_back(v2);
					meshAttributes.push_back(vertex4.x);
					meshAttributes.push_back(vertex4.y);
					meshAttributes.push_back(vertex4.z);
					meshAttributes.push_back(u2);
					meshAttributes.push_back(v1);
					meshAttributes.push_back(vertex3.x);
					meshAttributes.push_back(vertex3.y);
					meshAttributes.push_back(vertex3.z);
					meshAttributes.push_back(u2);
					meshAttributes.push_back(v2);
				}
			}
		}
	}
	verticesNumber = (meshAttributes.size() / 5) * 3;
	cloudNumber = cloudAttributes.size();
	std::sort(cloudAttributes.begin(), cloudAttributes.end(), sortZ);
}

void Skydome::setBuffers()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glUseProgram(mShaderProgram);

	GLint posAttrib = glGetAttribLocation(mShaderProgram, "vPosition");
	GLint uvAttrib = glGetAttribLocation(mShaderProgram, "vUV");
	//GLint normalAttrib = glGetAttribLocation(mShaderProgram, "vNormal");
	muVPLocation = glGetUniformLocation(mShaderProgram, "uVPMatrix");
	muInvViewLocation = glGetUniformLocation(mShaderProgram, "uInvViewMatrix");
	muTime = glGetUniformLocation(mShaderProgram, "uTime");
	muRadius = glGetUniformLocation(mShaderProgram, "uRadius");
	glGenVertexArrays(1, &mVao);
	glBindVertexArray(mVao);

	//Generate & bind vbo
	glGenBuffers(1, &mVbo);
	glBindBuffer(GL_ARRAY_BUFFER, mVbo);
	glBufferData(GL_ARRAY_BUFFER, meshAttributes.size() * sizeof(GLfloat), &meshAttributes[0], GL_STATIC_DRAW);

	// Get Attrib location
	/* glEnableVertexAttribArray(terrainPosAttrib); */
	/* glVertexAttribPointer(terrainPosAttrib, 3, GL_FLOAT, GL_FALSE, 24, 0); */
	/* glEnableVertexAttribArray(terrainNormalAttrib); */
	/* glVertexAttribPointer(terrainNormalAttrib, 3, GL_FLOAT, GL_FALSE, 24, reinterpret_cast<const GLvoid*>(12)); */

	// First all positions, then all normals
	glEnableVertexAttribArray(posAttrib);
	glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(uvAttrib);
	glVertexAttribPointer(uvAttrib, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	/* glVertexAttribPointer(terrainPosAttrib, 3, GL_FLOAT, GL_FALSE, 0, 0); */
//	if(mUseNormals)
//	{
//		glEnableVertexAttribArray(normalAttrib);
//		glVertexAttribPointer(normalAttrib, 3, GL_FLOAT, GL_FALSE, 12, reinterpret_cast<const GLvoid*>(mFloatsPerVertex/2 * mTotalVertices * sizeof(GLfloat)));
//	}


	//Unbind
	glBindVertexArray(0);
	/* delete mVertices; */
	/* delete mIndices; */
	//glBindBuffer(GL_ARRAY_BUFFER, 0);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}
