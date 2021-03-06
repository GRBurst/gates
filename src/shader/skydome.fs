#version 410 core
out vec4 color;
in vec2 fUV;
uniform float uTime;
uniform float uRadius;
uniform sampler3D textureSky;


#define PI 3.141592653589793238462643383279

void main()
{	
	//vec3 f3DUV= vec3(fUV, uTime / 64.0);
	float scaleCoor = fUV.x * 2 - 1;
	float gauss = exp(-scaleCoor * scaleCoor);
	vec4 skyColor =vec4(cos(fUV.x * 6.282) * 0.02 + 0.2314, cos(fUV.x * 6.282) * 0.08 + 0.4369, cos(fUV.x * 6.282) * 0.1666 + 0.8333, 1.0)* (cos(uTime/60) + 1.75) / 2.75 ;
 	//color = mix(vec4(texture(textureSky, f3DUV).r, texture(textureSky, f3DUV).r, texture(textureSky, f3DUV).r, 1.0), vec4(0.1, 0.22, 0.6, 1.0), 0.4 );
 	float testVal = texture(textureSky, vec3(fUV, uTime / 64)).r;
 	/* color = mix(1.5*vec4(fUV.x * 0.71, 0.15 + fUV.x * 0.1 + fUV.x * 0.23, fUV.x * 0.23 + 0.66, 1.0), vec4(texture(textureSky, vec3(fUV, uTime / 64)).rrr, 1.0), 0.7); */
    if (uRadius > 82 ){
    	color = mix(skyColor, vec4(texture(textureSky, vec3(fUV, uTime / 112)).rrr, 1.0 * gauss), smoothstep(0.5, 0.9, testVal  * gauss));
    	 //*/color = vec4(c0s(fUV.x * 6.282) * 0.05 + 0.2314, cos(fUV.x * 6.282) * 0.12 + 0.4369, cos(fUV.x * 6.282) * 0.2666 + 0.7333, 1.0);
    	
    //	color = mix(vec4(cos(fUV.x * 6.282) * 0.05 + 0.2314, cos(fUV.x * 6.282) * 0.12 + 0.4369, cos(fUV.x * 6.282) * 0.2666 + 0.7333, 1.0), vec4(texture(textureSky, vec3(fUV, uTime / 64)).rrr, 1), smoothstep(0.6, 0.9, texture(textureSky, vec3(fUV, uTime / 64)).r) );
    }
    else if (uRadius > 78)
    	{ 
    		color = mix(skyColor, vec4(texture(textureSky, vec3(fUV, uTime / 128)).rrr, 0.8 * gauss), smoothstep(0.6, 1.0, testVal  * gauss));
    	}
    	else
    	{ 
    		color = mix(skyColor, vec4(texture(textureSky, vec3(fUV, uTime / 96)).rrr, 0.7 * gauss), smoothstep(0.5, 0.8, testVal  * gauss));
    	}
    //color = mix(vec4(cos(fUV.x * 6.282) * 0.05 + 0.2314, cos(fUV.x * 6.282) * 0.12 + 0.4369, cos(fUV.x * 6.282) * 0.2666 + 0.7333, 1.0), vec4(0.8, 0.8, 0.8, 0.7), smoothstep(0.6, 0.9, texture(textureSky, vec3(fUV, uTime / 64)).r) );
    /* color = vec4(0.2314, 0.5569, 0.7333, 1.0); */
 	//color = vec4(fUV.x * 0.71, 0.15 + fUV.x * 0.3 + fUV.x * 0.23, fUV.x * 0.13 + 0.66, 1.0);
}
