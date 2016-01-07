#version 410 core
layout (location = 0) in vec3 vPosition;
in vec4 vColor;
uniform mat4 ModelViewProjectionMatrix;
void main()
{

    gl_Position = vec4(vPosition, 1.0);
}
