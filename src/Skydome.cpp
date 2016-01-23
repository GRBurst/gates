/*
 * Skydome.cpp
 *
 *  Created on: Jan 22, 2016
 *      Author: adrian
 */

#include "Skydome.h"

Skydome::Skydome(GLint shaderProgram) : mesh(0)
{
	// TODO Auto-generated constructor stub
	mShaderProgram = shaderProgram;
}

Skydome::~Skydome()
{
	// TODO Auto-generated destructor stub
}

void Skydome::draw()
{
	glUseProgram(mShaderProgram);

	glUniformMatrix4fv(muVPLocation, 1, GL_FALSE, value_ptr(mVPMatrix));
	glBindVertexArray(mVao);
	glDrawArrays(GL_TRIANGLES, 0, mesh.size() );

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
	for( int t = 0 ; t < meridians ; t++ )
	{
		float theta1 = ( (float)(t)/meridians )* glm::pi<float>();
		float theta2 = ( (float)(t+1)/meridians )* glm::pi<float>();

		for( int p = 0 ; p < azimuths ; p++ )
		{
			float phi1 = ( (float)(p)/azimuths )*2*glm::pi<float>() ; // azimuth goes around 0 .. 2*PI
			float phi2 = ( (float)(p+1)/azimuths )*2*glm::pi<float>() ;

			//phi2   phi1
			// |      |
			// 2------1 -- theta1
			// |\ _   |
			// |    \ |
			// 3------4 -- theta2
			//

			glm::vec3 vertex1 = glm::vec3(r * sin(theta1) * cos(phi1), r * sin(theta1) * sin(phi1), r * cos(theta1));
			glm::vec3 vertex2 = glm::vec3(r * sin(theta1) * cos(phi2), r * sin(theta1) * sin(phi2), r * cos(theta1));
			glm::vec3 vertex3 = glm::vec3(r * sin(theta2) * cos(phi2), r * sin(theta2) * sin(phi2), r * cos(theta2));
			glm::vec3 vertex4 = glm::vec3(r * sin(theta2) * cos(phi1), r * sin(theta2) * sin(phi1), r * cos(theta2));

			for(int i = 0; i < 3; i++)
			{
				if( t == 0 )
				{// top cap
					mesh.push_back( vertex1);
					mesh.push_back( vertex3);
					mesh.push_back( vertex4);
				}
				else
				{
					if( t + 1 == meridians ) //end cap
					{
						mesh.push_back( vertex3);
						mesh.push_back( vertex1);
						mesh.push_back( vertex2);
					}
					else
					{
					  // body, facing OUT:
						mesh.push_back( vertex1); //t1p1, t2p2, t2p1
						mesh.push_back( vertex2);
						mesh.push_back( vertex4);
						mesh.push_back( vertex2); //t1p1, t2p2, t2p1
						mesh.push_back( vertex3);
						mesh.push_back( vertex4);
					}
				}
			}
		}
	}
}

void Skydome::setBuffers()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glUseProgram(mShaderProgram);

	GLint posAttrib = glGetAttribLocation(mShaderProgram, "vPosition");
	//GLint normalAttrib = glGetAttribLocation(mShaderProgram, "vNormal");
	muVPLocation = glGetUniformLocation(mShaderProgram, "uVPMatrix");
	muInvViewLocation = glGetUniformLocation(mShaderProgram, "uInvViewMatrix");


	//Generate & bind vao
	glGenVertexArrays(1, &mVao);
	glBindVertexArray(mVao);

	//Generate & bind vbo
	glGenBuffers(1, &mVbo);
	glBindBuffer(GL_ARRAY_BUFFER, mVbo);
	glBufferData(GL_ARRAY_BUFFER, mesh.size() * 3 * sizeof(GLfloat), &mesh[0][0], GL_STATIC_DRAW);

	// Get Attrib location
	/* glEnableVertexAttribArray(terrainPosAttrib); */
	/* glVertexAttribPointer(terrainPosAttrib, 3, GL_FLOAT, GL_FALSE, 24, 0); */
	/* glEnableVertexAttribArray(terrainNormalAttrib); */
	/* glVertexAttribPointer(terrainNormalAttrib, 3, GL_FLOAT, GL_FALSE, 24, reinterpret_cast<const GLvoid*>(12)); */

	// First all positions, then all normals
	glEnableVertexAttribArray(posAttrib);
	glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 12, 0);
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
