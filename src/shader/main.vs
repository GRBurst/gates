#version 410 core
layout (location = 0) in vec3 vPosition;
in vec2 vUV;

uniform mat4 M;
uniform mat4 V;
uniform mat4 P;
uniform mat4 MVP;
uniform mat4 MV;
uniform mat3 MV3;
uniform vec3 light_position;
uniform vec3 camera_position;
out vec3 vColor;

void main()
{
	vColor = vec3(1.0, 0.0, 0.0);
    gl_Position = MVP * vec4(vPosition, 1.0);

}
