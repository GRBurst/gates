#version 410 core
layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec3 vNormal;
layout (location = 2) in vec3 vTangent;
layout (location = 3) in vec3 vBitangent;
layout (location = 4) in vec2 vUV;


uniform sampler2D sHeightMap;
uniform sampler2D sNormalMap;
uniform sampler2D sWhiteNoise;

out vec3 vPositionCS;
out vec2 vUVCS;
out vec3 vNormalCS;
out vec3 vTangentCS;
out vec3 vBitangentCS;



void main()
{


    vUVCS = vUV;
    
    float vHeight = vPosition.y;
	if (vHeight/3.8 < 0.06) 
	{
		vHeight = 0.06*3.1;
		vNormalCS = vec3(0, 1, 0);
	}
	else
		vNormalCS = vNormal;
        

    vPositionCS = vec3(vPosition.x, vHeight, vPosition.z);
    vTangentCS = vTangent;
    vBitangentCS = vBitangent;
    
    //fColor = vec3(y, y, y);
    //fColor = vec3(calculateUV(), height);
    //fColor = vec3(calculateUV(), 0.0);
    //fColor = vec3(0.0, 0.0, texture(heightMap, calculateUV()).r);
    //pos.y = texture(heightMap, vec2(0.5, 0.7)).r;
//    gl_Position = uVPMatrix * vec4(vPosition, 1.0);
    //gl_Position = uVPMatrix * vec4(vPosition.x, 0.0, -vPosition.y, 1.0);
}

