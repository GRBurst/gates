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
in mat3 invTBN;

uniform sampler2D sHeightMap;
uniform sampler2D sNormalMap;
uniform sampler2D sWhiteNoise;
uniform mat4 uVMatrix;
uniform mat4 uVPMatrix;
uniform int uHeightMapTerrainRatio;
uniform int uDrawGrid;
uniform vec3 uRayTerrainIntersection;
uniform float uEditMode;
uniform float uModifyRadius;
uniform vec3 uCamPos;
uniform vec3 uLightPos;

vec3 lightCol = vec3(1.0, 1.0, 0.95);
out vec4 color;

bool doHighLight()
{
    if(uEditMode < 2.0)
    {
        float radius = length(vec2(uRayTerrainIntersection.x - wPos.x, uRayTerrainIntersection.z - wPos.z));
        if((radius < uModifyRadius + 0.1) && (radius > uModifyRadius )) return true;
    }

    return false;
}

/* vec3 calcNormal( vec3 pos, float t ) */
/* { */
/*     vec2  eps = vec2( 0.002*t, 0.0 ); */
/*     return normalize( vec3( terrainH(pos.xz-eps.xy) - terrainH(pos.xz+eps.xy), */
/*                 2.0*eps.x, */
/*                 terrainH(pos.xz-eps.yx) - terrainH(pos.xz+eps.yx) ) ); */
/* } */

void main()
{

    vec3 normal_ts      = texture2D(sNormalMap, 2*fUV).rgb * 2.0f - 1.0f;
    vec3 light_ts       = normalize(fLightDir1_ts);
    vec3 eye_ts         = normalize(fEyeDir_ts);
    vec3 reflection_ts  = reflect(-light_ts, normal_ts);
    float cosTheta_ts   = clamp(dot (normal_ts, light_ts), 0, 1);
    float cosAlpha_ts   = clamp( dot( eye_ts, reflection_ts ), 0, 1 );

    float light1Power   = 8.0;
    vec3 pNormal_ws     = normalize(fNormal);

    /* vec3 fLightD_ws     = vec3( uLightPos - wPos ); */
    vec3 fLightD_ws     = vec3( 1, 2, 1 );
    float dist_ws       = length(fLightD_ws);
    fLightD_ws          = normalize(fLightD_ws);
    float intDif_ws     = max(dot(fLightD_ws, pNormal_ws), 0.0);
    vec3 dif_ws         = intDif_ws * vec3(7.0, 5.0, 3.0);

    float terrainHeight = wPos.y/3.8; //texture2D(sHeightMap, fUV).r;
    vec3 terrainNormal  = pNormal_ws;
    float normalHeight  = pNormal_ws.y;

    // light
    vec3 light      = lightCol * light1Power / (dist_ws * dist_ws);
    vec3 diffLight  = light * intDif_ws;

    // Base colors
    vec3 waterBaseColor1    = vec3( 0.52, 0.67, 0.64 );//brighter... //vec3( 0.15, 0.43, 0.48 );//vec3( 0.03, 0.06, 0.8 );
    vec3 waterBaseColor2    = vec3( 0.15, 0.43, 0.48 );//more blue
    vec3 grassBaseColor1    = vec3( 0.32, 0.58, 0.27 );//more green //vec3( 0.15, 0.60, 0.28 );
    vec3 grassBaseColor2    = vec3( 0.57, 0.80, 0.49 );//brighter/dry
    vec3 rockBaseColor1     = vec3( 0.59, 0.39, 0.12 );
    vec3 rockBaseColor2     = vec3( 0.69, 0.49, 0.22 );
    vec3 snowBaseColor      = vec3( 0.85, 0.85, 0.90 );


    // water
    float waterIntensity    = clamp(terrainHeight, 0.0, 0.2) + 0.8;
    vec3 waterColor1        = ( mix( waterBaseColor2, waterBaseColor1, smoothstep( 0.01, 0.08, ( terrainHeight ) )));
    vec3 waterColor2        = ( waterIntensity * mix( waterColor1, waterBaseColor1*texture2D(sWhiteNoise, fUV*0.17).r, smoothstep( 0.05, 0.12, ( terrainHeight ) )));
    /* vec3 waterColor         = waterColor1 * diffLight; */

    // grass
    float grassIntensity    = (1.0 - terrainHeight) ;
    /* vec3 grassColor1        = ( grassIntensity * mix( grassBaseColor2, grassBaseColor1, smoothstep( 0.7, 0.9, normalHeight))); */
    vec3 grassColor1        = ( mix( grassBaseColor1, grassBaseColor1*(0.5*texture2D(sWhiteNoise, fUV*0.1).r+0.5), smoothstep( 0.0, 0.4, normalHeight)));
    vec3 grassColor2        = ( mix( grassBaseColor2, grassBaseColor2*(0.5*texture2D(sWhiteNoise, fUV*0.1).r+0.5), smoothstep( 0.0, 0.4, normalHeight)));
    /* vec3 grassColor         = grassColor1 * diffLight; */

    // rock
    float rockIntensity = (terrainHeight);
    vec3 rockColor1      = cosTheta_ts * ( mix( rockBaseColor1, rockBaseColor1*texture2D(sWhiteNoise, fUV*0.04).r, smoothstep( 0.1, 0.9, normalHeight)));
    vec3 rockColor2      = cosTheta_ts * ( mix( rockBaseColor2, rockBaseColor2*texture2D(sWhiteNoise, fUV*0.04).r, smoothstep( 0.1, 0.9, normalHeight)));
    /* vec3 rockColor           = rockColor1 * diffLight * cosTheta_ts; */

    // snow
    float yNormal       = terrainNormal.y + (normal_ts).y;
    float snowIntensity = clamp(1.5*terrainHeight, 0.5, 1.0);
    vec3 snowColor1      = ( snowIntensity * mix( snowBaseColor, snowBaseColor*1.1, smoothstep( 0.9, 1.0, (1.0-normalHeight))));
    vec3 snowColor2      = ( snowIntensity * mix( snowColor1, snowColor1*texture2D(sWhiteNoise, fUV*0.14).r, smoothstep( 0.3, 0.7, (1.0-normalHeight))));
    vec3 snowColor           = snowColor1 * diffLight;

    /* vec3 resColor   = mix( waterColor, grassColor, step( 1.1 - waterIntensity, terrainHeight )); */
    vec3 resColor   = mix( waterColor1, waterColor2,    smoothstep( 0.00,   0.06,   terrainHeight           ));
    resColor        = mix( resColor,    grassColor1,    smoothstep( 0.04,   0.08,   terrainHeight  ));
    resColor        = mix( resColor,    grassColor2,    smoothstep( 0.08,   0.12,    terrainHeight*yNormal           ));
    resColor        = mix( resColor,    rockColor2,     smoothstep( 0.20,   0.35,    terrainHeight           ));
    resColor        = mix( resColor,    rockColor1,     smoothstep( 0.10,   0.12,    (terrainHeight*1.1)*(1-yNormal)           ));
    resColor        = mix( resColor,    snowColor1,     smoothstep( 0.4,    0.7,    terrainHeight*yNormal   ));
    /* resColor        = mix( resColor,    snowColor2,     smoothstep( 0.7,    1.0,    terrainHeight*yNormal   )); */
    color = vec4(resColor*diffLight, 1.0);
    /* color = vec4(diffLight * resColor, 1.0); */
    /* color = vec4((resColor * lightCol) * light1Power * (cosTheta_ts / (dist_ws * dist_ws)), 1.0); */

    /* // only rocks */
    /* vec3 resColor        = mix( snowColor, rockColor, smoothstep( 0.01, 0.1, terrainHeight) ); */
    /* resColor        = mix( resColor, snowColor, smoothstep( 0.3, 0.7, terrainHeight*yNormal) ); */
    /* color = vec4(resColor, 1.0); */

    /* // rock */
    /* float scale = 1.0f; */
    /* float r = 15.0f * texture2D( sHeightMap, (7.0/scale) * fUV ).r; */
    /* terrainColor1 = (r * 0.2 + 0.80) * 0.95 * mix( vec3(0.11, 0.08, 0.06), vec3(0.13, 0.12, 0.11), texture2D(sHeightMap, 0.00007 * vec2(fUV.x, wPos.y * 48.0f) /scale).r ); */
    /* terrainColor1 = mix( terrainColor1, 0.20 * vec3(0.46, 0.31, 0.16) * (0.50 + 0.50*r), smoothstep(0.70, 0.9, pNormal_ws.y) ); */
    /* terrainColor1 = mix( terrainColor1, 0.15 * vec3(0.32, 0.33, 0.12) * (0.25 + 0.75*r), smoothstep(0.95, 1.0, pNormal_ws.y) ); */

    /* // snow */
    /* float h = smoothstep(55.0, 80.0, wPos.y/scale + 25.0 * texture2D(sHeightMap, fUV).r ); */
    /* float e = smoothstep(1.0 - 0.5 * h, 1.0 - 0.1*h, pNormal_ws.y); */
    /* float o = 0.3 + 0.7*smoothstep(0.0, 0.1, pNormal_ws.x + h*h); */
    /* float s = h*e*o; */
    /* terrainColor1 = mix( terrainColor1, 0.29*vec3(0.62,0.65,0.7), smoothstep( 0.1, 0.9, s ) ); */


/*     float ambIntensity = clamp(0.5 + 0.5*pNormal_ws.y, 0.0, 1.0); */
/*     vec3 ambColor = ambIntensity * vec3(0.4, 0.6, 0.8) * 0.05; */

/*     float difIntensity = intDif_ws; */
/*     vec3 difColor = intDif_ws * terrainColor1; */

/*     float bacIntensity = 0.2*clamp( 0.2 + 0.8*dot( normalize( vec3(-light_ts.x, 0.0, light_ts.z ) ), normal_ts ), 0.0, 1.0 ); */
/*     vec3 bacColor = bacIntensity * vec3(0.40, 0.50, 0.60); */

/*     vec3 baseColor = ambColor + difColor + bacColor; */


/*     color = vec4(baseColor * (terrainColor1) * light1Power * (cosTheta_ts / (dist_ws * dist_ws)), 1.0); */
    /* color = vec4(bacColor * (difColor * lightCol) * light1Power * (cosTheta_ts / (dist_ws * dist_ws)), 1.0); */

    if((uDrawGrid == 1) || (uDrawGrid == 3))
        color = vec4(1.0, 0.0, 0.0, 1.0);
}
