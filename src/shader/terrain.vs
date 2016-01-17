#version 410 core
layout (location = 0) in vec2 vPosition;
//in vec2 vUV;

uniform mat4 VPMatrix;

out vec3 vColor;
//out vec2 fUV;

void main()
{
    vColor = vec3(0.0, 0.0, 1.0);
    //fUV = vUV;
    gl_Position = VPMatrix * vec4(vPosition.x, 0.0, -vPosition.y, 1.0);
    /* gl_Position = VPMatrix * vec4(vPosition, 0.0, 1.0); */
}
