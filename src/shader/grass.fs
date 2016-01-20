#version 410 core
in vec3 vColor;
in vec2 texCoord;
uniform sampler2D textureMat;
//uniform sampler2D heightMap; 
out vec4 color;
in vec3 vcolor;
vec4 colorRes;
uniform float fAlphaTest;
uniform float fAlphaMultiplier; 

void main()
{
	//color = vec4(1.0, 1.0, 0.0, 1.0);
	vec4 texColor = texture2D(textureMat, texCoord);
	float fNewAlpha = texColor.a*fAlphaMultiplier;
	if(fNewAlpha < fAlphaTest)
		discard;
	if(fNewAlpha > 1.0f)
		fNewAlpha = 1.0f;	
	
    color = texture2D(textureMat, texCoord);
    //color = vec4(texCoord, 1.0, 0.5);
}
