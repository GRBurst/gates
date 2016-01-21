#version 410 core
in vec2 texCoord;
in vec3 vColor;
in float height;
uniform sampler2D textureMat;
uniform sampler2D heightMap; 
out vec4 color;
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
	float colorDeviation = 0.6f + ((height * 2) - 1) / 2.0f;	
	vec3  mixedColor = vec3(texColor) * vec3(colorDeviation, colorDeviation, colorDeviation);
    color = vec4(mixedColor.rgb, fNewAlpha);
    //color = vec4(texCoord, 1.0, 0.5);
}
