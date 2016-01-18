#version 430 core
in vec3 vColor;
in vec2 texCoord;
uniform sampler2D textureMat;
//uniform sampler2D heightMap; 
out vec4 color;
vec4 colorRes;

void main()
{
	
   color = vec4(texture2D(textureMat, texCoord).rgb, 1.0);
    //color = vec4(texCoord, 1.0, 0.5);
}