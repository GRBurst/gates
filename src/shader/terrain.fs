#version 410 core
in float fHeight;
//in vec3 fColor;
in vec2 fUV;
in vec3 fNormal;
in vec3 fPos;
in vec3 wPos;

/* layout(binding = 0) uniform sampler2D heightMap; */
uniform sampler2D heightMap;
uniform mat4 uVPMatrix;
uniform int uHeightMapTerrainRatio;
uniform int uDrawGrid;
uniform vec3 uRayTerrainIntersection;
uniform vec3 uCamPos;

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
bool doHighLight()
{
    vec2 ray = vec2(uRayTerrainIntersection.x - wPos.x, uRayTerrainIntersection.z - wPos.z);
    float diff = length(ray);
    if(diff < 5.0) return true;

    return false;
}
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

    vec3 mapColor = texture(heightMap, fUV).rrr;
    float mapHeight = mapColor.r;

    //vec3 mixColor = (mapColor + heightColor)/2;
    float mixHeight = (mapHeight + fHeight)/2;

    /* float r = sin(fPos.x) * cos(fPos.y); */
    /* float g = sin(fPos.y) * cos(fPos.z); */
    /* float b = sin(fPos.z) * cos(fPos.x); */

    vec3 terrainColor1, terrainColor2;
    float tHeight = fHeight * fHeight;

    if (fHeight < 0.1)
    {
        terrainColor1 = vec3(0.1, 0.15, 0.8) - vec3(0.0, 0.0, fHeight * 2);
    }
    else if (fHeight < (0.2 + fNormal.y/1.0))
    {
        terrainColor1 = vec3(0.1, 0.6, 0.1) * (0.1 + fHeight);
        /* terrainColor2 = vec3(0.8 * fHeight); */
        /* terrainColor1 = mix(terrainColor1, terrainColor2, fHeight/(2.0)); */
    }
    else if (fHeight < (2.2 - fNormal.y))
    {
        terrainColor1 = vec3(0.8 * fHeight);
        /* terrainColor2 = vec3(1.0); */
        /* terrainColor1 = mix(terrainColor1, terrainColor2, 0.2); */
    }
    else
    {
        terrainColor1 = vec3(fHeight);
    }

    /* if(ray()) color = vec4(0.0, 1.0, 1.0, 1.0); */
    /* else if(place()) color = vec4(); */
    if(doHighLight()) color = vec4(0.0, 1.0, 1.0, 1.0);
    else color = phong(terrainColor1);
    /* color = phong(terrainColor1); */

    //color = terrainColor * vec4(diff, 1.0);
    //color = vec4(diff, 1.0);
    //color = vec4(fNormal, 1.0);

    if((uDrawGrid == 1) || (uDrawGrid == 3))
        color = vec4(1.0, 0.0, 0.0, 1.0);
}
