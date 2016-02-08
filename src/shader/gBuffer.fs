#version 410 core
layout (location = 0) out vec4 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec3 gAlbedo;
/* layout (location = 3) out vec3 gAlbedoSpec; */

in vec2 fUV;
in vec3 fNormal;
in vec3 wPos;
in vec3 fPos;
in mat3 fTBN;

uniform sampler2D sNormalMap;
uniform sampler2D sWhiteNoise;
uniform vec3 uRayTerrainIntersection;
uniform float uNearPlane;
uniform float uFarPlane;
uniform float uEditMode;
uniform float uModifyRadius;

float getLineareDepth( float depth )
{
    float z = depth * 2.0 - 1.0; // Back to NDC 
    return (2.0 * uNearPlane * uFarPlane) / (uFarPlane + uNearPlane - z * (uFarPlane - uNearPlane));	
}

bool doHighLight()
{
    if(uEditMode < 2.0)
    {
        float radius = length(vec2(uRayTerrainIntersection.x - wPos.x, uRayTerrainIntersection.z - wPos.z));
        if((radius < uModifyRadius + 0.1) && (radius > uModifyRadius )) return true;
    }

    return false;
}

void main()
{
    gPosition.xyz   = fPos;
    gPosition.w     = getLineareDepth(gl_FragCoord.z);
    gNormal         = normalize(fNormal);

    if(doHighLight()) gAlbedo = vec3(uEditMode, 1.0-uEditMode, 1.0-uEditMode);
    else
    {
        vec3 normal_ts      = texture2D(sNormalMap, 2*fUV).rgb * 2.0f - 1.0f;
        float terrainHeight = wPos.y/3.8;
        vec3 terrainNormal  = gNormal;
        float normalHeight  = gNormal.y;

        // Base colors
        vec3 waterBaseColor1    = vec3( 0.18824, 0.39216, 0.58039 );
        vec3 waterBaseColor2    = vec3( 0.33725, 0.53725, 0.72157 );
        vec3 grassBaseColor1    = vec3( 0.32, 0.58, 0.27 );
        vec3 grassBaseColor2    = vec3( 0.57, 0.80, 0.49 );
        vec3 rockBaseColor1     = vec3( 0.59, 0.39, 0.12 );
        vec3 rockBaseColor2     = vec3( 0.69, 0.49, 0.22 );
        vec3 snowBaseColor      = vec3( 0.85, 0.85, 0.90 );

        // water
        vec3 waterColor1        = ( mix( waterBaseColor2, waterBaseColor1, smoothstep( 0.01, 0.08, ( terrainHeight ) )));
        vec3 waterColor2        = ( mix( waterColor1, waterBaseColor1*texture2D(sWhiteNoise, vec2(fUV.x*0.17, fUV.y)).r, smoothstep( 0.05, 0.12, ( terrainHeight ) )));

        // grass
        vec3 grassColor1        = ( mix( grassBaseColor1, grassBaseColor1*(0.5*texture2D(sWhiteNoise, fUV*0.1).r+0.5), smoothstep( 0.0, 0.4, normalHeight)));
        vec3 grassColor2        = ( mix( grassBaseColor2, grassBaseColor2*(0.5*texture2D(sWhiteNoise, vec2(fUV.x*2.1, fUV.y)).r+0.5), smoothstep( 0.0, 0.4, normalHeight)));

        // rock
        vec3 rockColor1      = ( mix( rockBaseColor1, rockBaseColor1*texture2D(sWhiteNoise, fUV*0.04).r, smoothstep( 0.1, 0.9, normalHeight)));
        vec3 rockColor2      = ( mix( rockBaseColor2, rockBaseColor2*texture2D(sWhiteNoise, fUV*2.00).r, smoothstep( 0.1, 0.9, normalHeight)));

        // snow
        float yNormal       = terrainNormal.y + (normal_ts).y;
        float snowIntensity = clamp(1.5*terrainHeight, 0.5, 1.0);
        vec3 snowColor1     = ( snowIntensity * mix( snowBaseColor, snowBaseColor*1.1, smoothstep( 0.9, 1.0, (1.0-normalHeight))));
        vec3 snowColor2     = ( snowIntensity * mix( snowColor1, snowColor1*texture2D(sWhiteNoise, fUV*0.14).r, smoothstep( 0.3, 0.7, (1.0-normalHeight))));

        vec3 resColor   = mix( waterColor1, waterColor2,    smoothstep( 0.00,   0.06,   terrainHeight           ));
        resColor        = mix( resColor,    grassColor1,    smoothstep( 0.04,   0.08,   terrainHeight  ));
        resColor        = mix( resColor,    grassColor2,    smoothstep( 0.08,   0.12,    terrainHeight*yNormal           ));
        resColor        = mix( resColor,    rockColor2,     smoothstep( 0.25,   0.35,    terrainHeight           ));
        resColor        = mix( resColor,    rockColor1,     smoothstep( 0.10,   0.12,    (terrainHeight*1.1)*(1-yNormal)           ));
        resColor        = mix( resColor,    snowColor1,     smoothstep( 0.4,    0.7,    terrainHeight*yNormal   ));
        resColor        = mix( resColor,    snowColor2,     smoothstep( 0.7,    1.0,    terrainHeight*yNormal   ));

        gAlbedo = resColor;
    }
}
