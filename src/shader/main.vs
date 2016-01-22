#version 410 core
layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec2 vUV;
layout (location = 2) in vec3 vNormal;
layout (location = 3) in vec3 vBitangent;
layout (location = 4) in vec3 vTangent;

uniform mat4 M;
uniform mat4 V;
uniform mat4 P;
uniform mat4 MVP;
uniform mat4 MV;
uniform mat3 MV3;
uniform vec3 light_position;
uniform vec3 camera_position;
out vec3 vColor;
out vec2 fUV;

void main()
{
	vColor = vec3(1.0, 0.0, 0.0);
    fUV = vUV;
    gl_Position = MVP * vec4(vPosition, 1.0);
}
