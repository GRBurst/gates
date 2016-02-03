#version 410 core
in float fHeight;
//in vec3 fColor;
in vec2 fUV;
in vec3 fNormal;
in vec3 fPos;
in vec3 wPos;
in vec3 fLightDir1_ts;
in vec3 fEyeDir_ts;
in vec3 tangent_cs;
in vec3 bitangent_cs;
in vec3 normal_cs;
in vec4 test;

/* layout(binding = 0) uniform sampler2D heightMap; */
uniform sampler2D heightMap;
uniform sampler2D sNormalMap;
uniform mat4 uVMatrix;
uniform mat4 uVPMatrix;
uniform int uHeightMapTerrainRatio;
uniform int uDrawGrid;
uniform vec3 uRayTerrainIntersection;
uniform float uEditMode;
uniform float uModifyRadius;
uniform vec3 uCamPos;
uniform vec3 uLightPos;

vec3 lightCol = vec3(1.0, 1.0, 1.0);

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
    if(uEditMode < 2.0)
    {
        float radius = length(vec2(uRayTerrainIntersection.x - wPos.x, uRayTerrainIntersection.z - wPos.z));
        if((radius < uModifyRadius + 0.1) && (radius > uModifyRadius )) return true;
    }

    return false;
}
vec3 water()
{
    return vec3(0.0,0.0,0.0);
}
vec4 phong(vec3 _color)
{
    vec3 normal_ts      = texture2D(sNormalMap, fUV).rgb * 2.0f - 1.0f;
    vec3 light_ts       = normalize(fLightDir1_ts);
    vec3 eye_ts         = normalize(fEyeDir_ts);
    vec3 reflection_ts  = reflect(-light_ts, normal_ts);
    float cosTheta_ts   = clamp(dot (normal_ts, light_ts), 0, 1);
    float cosAlpha_ts   = clamp( dot( eye_ts, reflection_ts ), 0, 1 );


    float light1Power   = 2500.0;
    vec3 pNormal_ws     = normalize(fNormal);

    vec3 fLightD_ws     = vec3( uLightPos - wPos );
    float dist_ws       = length(fLightD_ws);
    fLightD_ws          = normalize(fLightD_ws);
    float intDif_ws     = max(dot(fLightD_ws, pNormal_ws), 0.0);
    vec3 dif_ws         = vec3(intDif_ws * _color);

    /* float attenuation   = 1.0; // /(1.0+0.00000001*dist+0.00000000001*dist*dist); */
    /* return vec4(((dif) * attenuation), 1.0); //vec4(((amb + dif + spec) * attenuation), 1.0);    //vec4(pNormal, 1.0); */
    /* return (ambientColor + (diffuseColor * light1Color) * light1Power * (cosTheta / (dist * dist)) + specularColor * light1Color * light1Power * (pow(cosAlpha, 5) / (dist * dist))); */
    return vec4((dif_ws * lightCol) * light1Power * (cosTheta_ts / (dist_ws * dist_ws)), 1.0);
}

void main()
{
    //vec3 diff = dot(normalize(fNormal), normalize(fPos - lightPos)) * lightCol;

    vec3 mapColor = texture(heightMap, fUV).rrr;
    float mapHeight = mapColor.r;

    //vec3 mixColor = (mapColor + heightColor)/2;
    float mixHeight = (mapHeight + fHeight)/2;

    vec3 terrainColor1, terrainColor2;
    float tHeight = fHeight * fHeight;

    if (fHeight < 0.1)
    {
        terrainColor1 = vec3(0.1, 0.15, 0.8) - vec3(0.0, 0.0, fHeight * 2);
    }
    else if (fHeight < (0.2 + fNormal.y/1.0))
    /* else if (fHeight < (0.2)) */
    {
        terrainColor1 = vec3(0.1, 0.6, 0.1) * (0.1 + fHeight);
        /* terrainColor2 = vec3(0.8 * fHeight); */
        /* terrainColor1 = mix(terrainColor1, terrainColor2, fHeight/(2.0)); */
    }
    else if (fHeight < (2.2 - fNormal.y))
    /* else if (fHeight < (2.2)) */
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
    if(doHighLight()) color = vec4(uEditMode, 1.0-uEditMode, 1.0-uEditMode, 1.0);
    else color = phong(terrainColor1);
    /* color = phong(terrainColor1); */

    //color = terrainColor * vec4(diff, 1.0);
    //color = vec4(diff, 1.0);
    //color = vec4(fNormal, 1.0);

    if((uDrawGrid == 1) || (uDrawGrid == 3))
        color = vec4(1.0, 0.0, 0.0, 1.0);
}
