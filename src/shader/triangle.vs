#version 410 core
layout (location = 0) in vec3 vPosition;
//in vec4 vColor;
out vec3 vColor;
uniform mat4 ModelViewProjectionMatrix;
void main()
{
	vColor = vPosition / vPosition.z - vec3(1);
    gl_Position = vec4(vColor, 1.0);
}
