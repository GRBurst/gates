#version 410 core
out vec4 color;
in vec2 fUV;
uniform float uTime;

uniform sampler3D tClouds;

#define PI 3.141592653589793238462643383279

float gauss(float x){
	float sigmaSQ =5;
	float a = 1/(sqrt(2*PI * sigmaSQ));
	return a*exp(-((x-0) * (x-0)/(2*sigmaSQ)));
	
}

void main()
{	
	vec4 resColor;
	vec3 f3DUV= vec3(fUV, mod(uTime / 10, 1.0) );
	if (texture(tClouds, f3DUV).r < 0.6)
		resColor = vec4(0.0, 0.0, 0.0, 0.0);
	else
		resColor = vec4(texture(tClouds, f3DUV).r, texture(tClouds, f3DUV).r, texture(tClouds, f3DUV).r, gauss(fUV.x) + gauss(fUV.y));
	
 	color = resColor;
 	
 	//color = vec4(1.0, 0.0, 0.0, 1.0);
}
