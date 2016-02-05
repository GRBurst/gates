#version 410 core
layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec2 vUV;
out vec2 fUV;

uniform float uTime;
uniform mat4 uVPMatrix;
mat4 rotationMatrixY(float angle){
	float sinVal = sin(angle);
	float cosVal = cos(angle);
	float cosToOne = 1.0f -cosVal;
	
	return mat4(cosVal,										0,											 		sinVal,										 	0.0f,
				0,											1,		 											0,											 	0.0f,
				-sinVal,									0,											 		cosVal,									 		0.0f,
				0.0f,										0.0f,												0.0f,											1.0f);
}
void main(){
	fUV = vUV;
    gl_Position = uVPMatrix * rotationMatrixY(uTime / 60) * vec4(vPosition, 1.0);
}