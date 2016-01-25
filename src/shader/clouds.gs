#version 410 core

layout (points) in;
layout (triangle_strip, max_vertices = 4) out;


uniform mat4 gVP;
uniform mat4 uVPMatrix;
uniform float uTime;
//uniform float uScale;

out float height;
out vec2 fUV;

out vec3 fPosition;

#define PI 3.141592653589793238462643383279

mat4 rotationMatrix(vec3 axis, float angle){
	axis = normalize(axis);
	float sinVal = sin(angle);
	float cosVal = cos(angle);
	float cosToOne = 1.0f -cosVal;
	
	return mat4(cosVal + axis.x * axis.x * cosToOne,		axis.x * axis.y * cosToOne - axis.z * sinVal, 		axis.z * axis.x * cosToOne + axis.y * sinVal, 	0.0f,
				axis.x * axis.y * cosToOne + axis.z*sinVal,	cosVal + axis.y * axis.y * cosToOne,		 		axis.y * axis.z * cosToOne - axis.x * sinVal, 	0.0f,
				axis.z * axis.x * cosToOne - axis.y*sinVal,	axis.z * axis.y * cosToOne + axis.x * sinVal, 		cosVal + axis.z * axis.z  * cosToOne,	 		0.0f,
				0.0f,										0.0f,												0.0f,											1.0f);
}

void main()
{
	vec3 norm = normalize(vec3(gl_in[0].gl_Position));
	vec3 right = normalize(cross(norm, vec3(0, 1, 0)));
	vec3 up = normalize(cross(norm, right));
	fPosition = vec3(gl_in[0].gl_Position);
	mat4 M = rotationMatrix(vec3(0, 1, 0), (uTime / 512) * PI);
	gl_Position = uVPMatrix * M * (gl_in[0].gl_Position -vec4(20*norm + 10 * right - 5*up, 0));
	fUV = vec2(1, 0);	
	EmitVertex();
	
	gl_Position = uVPMatrix * M * (gl_in[0].gl_Position -vec4(20*norm - 10 * right - 5*up, 0));	
	fUV = vec2(0, 0);
	EmitVertex();
	
	gl_Position = uVPMatrix * M * (gl_in[0].gl_Position -vec4(20*norm + 10 * right + 5*up, 0));	
	fUV = vec2(1, 1);
	EmitVertex();
	
	gl_Position = uVPMatrix * M * (gl_in[0].gl_Position -vec4(20*norm - 10 * right + 5*up, 0));	
	fUV = vec2(0, 1);
	EmitVertex();
 		
    EndPrimitive();
	  
}

	

