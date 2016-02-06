#version 410 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vPosition;

// Output data ; will be interpolated for each fragment.
out vec2 UV;

uniform mat4 uVPMatrix;

void main()
{
    //gl_Position =  uVPMatrix * vec4(vPosition,1);
    gl_Position =  vec4(vPosition, 1);
    UV = (vPosition.xy+vec2(1,1))/2.0;
}

