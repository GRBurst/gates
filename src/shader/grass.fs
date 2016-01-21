#version 410 core
in vec2 texCoord;
in vec3 vColor;
in vec3 vNormal;
in vec3 vPosition;
in float height;
uniform sampler2D textureMat;
uniform sampler2D heightMap; 
out vec4 color;
vec4 colorRes;


uniform float fAlphaTest;
uniform float fAlphaMultiplier; 

vec3 lightCol = vec3(1.0, 1.0, 1.0);
vec3 lightPos = vec3(0.0, 10.0, 0.0);

vec4 phong(vec4 _color)
{
    float dist          = length(vec3(vPosition - lightPos));
    float attenuation   = 1.0; // /(1.0+0.00000001*dist+0.00000000001*dist*dist);
    vec3 fNormal             = normalize(vNormal);
    //vec3 amb            = vec3(texture(uTexture, vTexCoord).rgb * vec3(uLightAmb0));
    vec3 fLightD        = vec3(normalize(lightPos - vPosition));
    float intDif        = max(dot(fLightD, fNormal), 0.0);
    vec3 dif            = vec3(intDif *vec3( _color));
    //vec3 view           = normalize(-vec3(fPos));
    //vec3 fLightR        = normalize(reflect(-fLightD, pNormal));
    //float intSpec       = pow(max(dot(view, fLightR), 0.0), uMatShine0);
    //vec3 spec           = vec3(intSpec * uMatSpec0 * uLightSpec0);
    return vec4(((dif) * attenuation), _color.a); //vec4(((amb + dif + spec) * attenuation), 1.0);    //vec4(pNormal, 1.0);
}

void main()
{
	color = vec4(1.0, 1.0, 0.0, 1.0);
	vec4 texColor = texture2D(textureMat, texCoord);
	float fNewAlpha = texColor.a*fAlphaMultiplier;
	if(fNewAlpha < fAlphaTest)
		discard;
	if(fNewAlpha > 1.0f)
		fNewAlpha = 1.0f;
	float colorDeviation = 0.6f + ((height * 2) - 1) / 2.0f;	
	vec3  mixedColor = vec3(texColor) * vec3(colorDeviation, colorDeviation, colorDeviation);
    //color = vec4(texColor.rgb, fNewAlpha);
    //color = vec4(texCoord, 1.0, 0.5);
}
