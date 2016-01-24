/*
 * Clouds.h
 *
 *  Created on: Jan 24, 2016
 *      Author: adrian
 */

#ifndef SRC_CLOUDS_H_
#define SRC_CLOUDS_H_
#include <GL/glew.h>
#include <vector>
#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>
#include "Camera.h"
#include "Texture.h"

class Clouds
{
public:
	Clouds( GLint &shaderProgram, int cloudNumber, float* geometry, Camera* cam);
	virtual ~Clouds();
	void draw();
	void setBuffers();
	void loadTexture(float* textureData, int width, int height, int depth);
private:
	int mCloudNumber;
	GLint mShaderProgram;
	GLuint mVao, mVbo;
	GLuint muVPLocation;
	//GLuint muInvViewLocation;
	GLuint muTime;
	float* mGeometry;
	Camera* camera;
};

#endif /* SRC_CLOUDS_H_ */
