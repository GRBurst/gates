#version 410 core
in vec3 fColor;
in vec2 fUV;
in vec3 fNormal;
in vec3 fPos;

/* layout(binding = 0) uniform sampler2D heightMap; */
uniform sampler2D heightMap;
uniform mat4 uVPMatrix;
uniform int uHeightMapTerrainRatio;
uniform int uDrawGrid;

vec3 lightCol = vec3(1.0, 1.0, 1.0);
vec3 lightPos = vec3(0.0, 100.0, 0.0);

out vec4 color;
/* vec4 phong() */
/* { */
/*     float dist          = length(vec3(objPos - uViewMatrix * uLightPos0)); */
/*     float attenuation   = 1.0/(1.0+0.00000001*dist+0.00000000001*dist*dist); */
/*     vec3 fNormal        = normalize(vNormal); */
/*     vec3 amb            = vec3(texture(uTexture, vTexCoord).rgb * vec3(uLightAmb0)); */
/*     vec3 fLightD        = vec3(normalize(uLightPos0 - objPos)); */
/*     float intDif        = max(dot(fLightD, fNormal), 0.0); */
/*     vec3 dif            = vec3(intDif * texture(uTexture, vTexCoord).rgb * vec3(uLightDif0)); */
/*     vec3 view           = normalize(uEyePos - vec3(objPos)); */
/*     vec3 fLightR        = normalize(reflect(-fLightD, fNormal)); */
/*     float intSpec       = pow(max(dot(view, fLightR), 0.0), uMatShine0); */
/*     vec3 spec           = vec3(intSpec * uMatSpec0 * uLightSpec0); */
/*     oColor = vec4(((amb + dif + spec) * attenuation), 1.0); //vec4(((amb + dif + spec) * attenuation), 1.0);    //vec4(fNormal, 1.0); */
/* } */
vec4 phong(vec3 _color)
{
    float dist          = length(vec3(fPos - lightPos));
    float attenuation   = 1.0; // /(1.0+0.00000001*dist+0.00000000001*dist*dist);
    vec3 pNormal             = normalize(fNormal);
    //vec3 amb            = vec3(texture(uTexture, vTexCoord).rgb * vec3(uLightAmb0));
    vec3 fLightD        = vec3(normalize(lightPos - fPos));
    float intDif        = max(dot(fLightD, pNormal), 0.0);
    vec3 dif            = vec3(intDif * _color);
    //vec3 view           = normalize(-vec3(fPos));
    //vec3 fLightR        = normalize(reflect(-fLightD, pNormal));
    //float intSpec       = pow(max(dot(view, fLightR), 0.0), uMatShine0);
    //vec3 spec           = vec3(intSpec * uMatSpec0 * uLightSpec0);
    return vec4(((dif) * attenuation), 1.0); //vec4(((amb + dif + spec) * attenuation), 1.0);    //vec4(pNormal, 1.0);
}
void main()
{
    //vec3 diff = dot(normalize(fNormal), normalize(fPos - lightPos)) * lightCol;

    vec3 colorRes = fColor;
    //vec3 colorRes = texture(heightMap, fUV).rrr;
    float initColor = colorRes.r;

    vec4 terrainColor;
    if (initColor < 0.1){
        terrainColor = vec4(0.1, 0.15, 0.8, 1.0) - (vec4(0.0, 0.0, colorRes.r * 2, 0.0) );
    }
    else if (initColor >= 0.1 && initColor < 0.8){
        terrainColor =vec4(0.1, 0.6, 0.1, 1.0) * vec4(colorRes.rrr, 1.0);
    }
    else if (initColor >= 0.8 && initColor < 1.2){
        terrainColor = vec4(0.8, 0.8, 0.8, 1.0) * vec4(colorRes.rrr, 1.0);
    }
    else
    {
        terrainColor = vec4(1.0, 1.0, 1.0, 1.0) * vec4(colorRes.rrr, 1.0);
    }

    color = phong(vec3(terrainColor));
    //color = terrainColor * vec4(diff, 1.0);
    //color = vec4(diff, 1.0);
    //color = vec4(fNormal, 1.0);

    if((uDrawGrid == 1) || (uDrawGrid == 3))
        color = vec4(1.0, 0.0, 0.0, 1.0);
}
