/*
 * Skydome.cpp
 *
 *  Created on: Jan 22, 2016
 *      Author: adrian
 */

#include "Skydome.h"

Skydome::Skydome(GLint shaderProgram, Camera* camera) : meshAttributes(0), mVao(0), mVbo(0), mShaderProgram(0),
														muVPLocation(0), muInvViewLocation(0),
														camera(0), verticesNumber(0)
{
	// TODO Auto-generated constructor stub
	mShaderProgram = shaderProgram;
	this->camera = camera;
}

Skydome::~Skydome()
{
	// TODO Auto-generated destructor stub
}

void Skydome::loadTexture(float* textureData, int width, int height){
	Texture* texture = new Texture();
	texture->bind();
	texture->setData(textureData, width, height);
	texture->loadCommonOptions();
	texture->linkTexture(mShaderProgram, "textureSky");
}

void Skydome::draw()
{
	glUseProgram(mShaderProgram);

	glUniformMatrix4fv(muVPLocation, 1, GL_FALSE, value_ptr(camera->getVPMatrix()));
	glBindVertexArray(mVao);
	glDrawArrays(GL_TRIANGLES, 0, verticesNumber );

	/* if (textureCount >= 1) */
	/* 	texture->bind(); */
	/* if (textureCount >= 2) */
	/* 	normalmap->bind(); */
	/* if (textureCount >= 3) */
	/* 	heightmap->bind(); */
	//cout << shaderProgram << endl;
	//glBindVertexArray(0);
	//glUseProgram(0);

	glBindVertexArray(0);
}

void Skydome::generateGeometry(float r, int azimuths, int meridians)
{
	//UV Generation is spherical
	std::cout << "sfklnjsedfikl";
	//meshAttributes.resize(azimuths * 30 + (meridians + 1) * azimuths * 30); //azimuths * 2 * 3 * ( 3+ 2) + (meridians - 2) * azimuths * 6 * (3 + 2)
	//verticesNumber = azimuths * 18 + (meridians + 1) * azimuths * 18;
	int i = 0;
	for( int t = 0 ; t < meridians ; t++ )
	{
		float u1 = (float)(t)/meridians;
		float u2 = (float)(t+1)/meridians;
		float theta1 = u1 * glm::pi<float>();
		float theta2 = u2 * glm::pi<float>();

		for( int p = 0 ; p < azimuths ; p++ )
		{
			float v1 =  (float)(p)/azimuths;
			float v2 = (float)(p+1)/azimuths;
			float phi1 = v1 * 2 * glm::pi<float>();
			float phi2 = v2 * 2 * glm::pi<float>();

			glm::vec3 vertex1 = glm::vec3(r * sin(theta1) * cos(phi1), r * cos(theta1), r * sin(theta1) * sin(phi1)); //z and y switch to have caps of sphere top and bottom
			glm::vec3 vertex2 = glm::vec3(r * sin(theta1) * cos(phi2), r * cos(theta1), r * sin(theta1) * sin(phi2));
			glm::vec3 vertex3 = glm::vec3(r * sin(theta2) * cos(phi2), r * cos(theta2), r * sin(theta2) * sin(phi2));
			glm::vec3 vertex4 = glm::vec3(r * sin(theta2) * cos(phi1), r * cos(theta2), r * sin(theta2) * sin(phi1));
			std::cout << i << std::endl;

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
			i++;

		}
	}
	std::cout << "DORT";
}

void Skydome::setBuffers()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glUseProgram(mShaderProgram);

	GLint posAttrib = glGetAttribLocation(mShaderProgram, "vPosition");
	GLint uvAttrib = glGetAttribLocation(mShaderProgram, "vUV");
	//GLint normalAttrib = glGetAttribLocation(mShaderProgram, "vNormal");
	muVPLocation = glGetUniformLocation(mShaderProgram, "uVPMatrix");
	muInvViewLocation = glGetUniformLocation(mShaderProgram, "uInvViewMatrix");

	verticesNumber = (meshAttributes.size() / 5) * 3;
	//Generate & bind vao
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
