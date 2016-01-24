#version 410 core
out vec4 color;
in vec2 fUV;
uniform float uTime;

uniform sampler3D textureSky;


#define PI 3.141592653589793238462643383279

void main()
{	
	vec3 f3DUV= vec3(fUV, uTime / 64.0);
 	color = mix(vec4(texture(textureSky, f3DUV).r, texture(textureSky, f3DUV).r, texture(textureSky, f3DUV).r, 1.0), vec4(0.1, 0.22, 0.6, 1.0), 0.4 );
 	//color = vec4(0.0, 1.0, 1.0, 1.0);
}
