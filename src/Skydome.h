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
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
class Skydome
{
public:
	Skydome(GLint shaderProgram);
	virtual ~Skydome();
	void setVPMatrix(glm::mat4 vp) { this->mVPMatrix = vp; };
	void draw();
	void generateGeometry(float r, int azimuths, int meridians);
	void setBuffers();
private:
	std::vector<glm::vec3> mesh;
	GLuint mVao;
	GLuint mVbo;
	GLint mShaderProgram;
	GLuint muVPLocation;
	glm::mat4 mVPMatrix;
	GLuint muInvViewLocation;
	glm::mat3 mInvViewMatrix;
};

#endif /* SRC_SKYDOME_H_ */
