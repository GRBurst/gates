#version 410 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec2 vPosition;
layout (location = 1) in vec2 texCoords;

// Output data ; will be interpolated for each fragment.
out vec2 UV;

uniform mat4 uVPMatrix;

void main()
{
    //gl_Position =  uVPMatrix * vec4(vPosition,1);
    gl_Position =  vec4(vPosition, 0, 1);
    UV = (vPosition.xy+vec2(1,1))/2.0;
}

