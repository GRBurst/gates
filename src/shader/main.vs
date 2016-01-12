#version 410 core

uniform mat4 mvp;

in vec3 aPosition;

void main()
{

    gl_Position = mvp * vec4(aPosition, 1.0);

}
