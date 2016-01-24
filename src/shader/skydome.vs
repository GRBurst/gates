#version 410 core
layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec2 vUV;
out vec2 fUV;

uniform mat4 uVPMatrix;

void main(){
	fUV = vUV;
    gl_Position = uVPMatrix * vec4(vPosition, 1.0);
}