#version 410 core
out vec4 color;
in vec2 fUV;
uniform float uTime;

uniform sampler3D textureSky;


#define PI 3.141592653589793238462643383279

void main()
{	
	//vec3 f3DUV= vec3(fUV, uTime / 64.0);
 	//color = mix(vec4(texture(textureSky, f3DUV).r, texture(textureSky, f3DUV).r, texture(textureSky, f3DUV).r, 1.0), vec4(0.1, 0.22, 0.6, 1.0), 0.4 );
 	
 	/* color = mix(1.5*vec4(fUV.x * 0.71, 0.15 + fUV.x * 0.1 + fUV.x * 0.23, fUV.x * 0.23 + 0.66, 1.0), vec4(texture(textureSky, vec3(fUV, uTime / 64)).rrr, 1.0), 0.7); */
    color = mix(vec4(0.2314, 0.5569, 0.7333, 1.0), vec4(0.8, 0.8, 0.8, 0.7), smoothstep(0.6, 0.9, texture(textureSky, vec3(fUV, uTime / 64)).r) );
    /* color = vec4(0.2314, 0.5569, 0.7333, 1.0); */
 	//color = vec4(fUV.x * 0.71, 0.15 + fUV.x * 0.3 + fUV.x * 0.23, fUV.x * 0.13 + 0.66, 1.0);
}
