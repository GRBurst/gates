#version 410 core
layout (location = 0) in vec3 vPosition;

uniform mat4 M;
uniform mat4 V;
uniform mat4 P;
uniform mat4 MVP;
uniform mat4 MV;
uniform mat3 MV3;
uniform vec3 light_position;
uniform vec3 camera_position;

void main()
{

    gl_Position = mvp * vec4(vPosition, 1.0);

}
