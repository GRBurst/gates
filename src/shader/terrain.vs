#version 410 core
layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec3 vNormal;
layout (location = 2) in vec3 vTangent;
layout (location = 3) in vec3 vBitangent;
layout (location = 4) in vec2 vUV;

uniform sampler2D heightMap;
uniform mat4 uVMatrix;
uniform mat4 uVPMatrix;
uniform mat3 uInvViewMatrix;
uniform int uHeightMapTerrainRatio;
uniform int uDrawGrid;
uniform vec3 uRayTerrainIntersection;
uniform float uEditMode;
uniform float uModifyRadius;
uniform vec3 uCamPos;
uniform vec3 uLightPos;

out float fHeight;
out vec2 fUV;
out vec3 fNormal;
out vec3 fPos;
out vec3 wPos;
out vec3 fLightDir1_ts;
out vec3 fEyeDir_ts;
out vec3 tangent_cs;
out vec3 bitangent_cs;
out vec3 normal_cs;
out vec4 test;

float getHightScale()
{

    vec2 ray = vec2(uRayTerrainIntersection.x - vPosition.x, uRayTerrainIntersection.z - vPosition.z);
    float diff = length(ray);
    if(diff < uModifyRadius) return (uModifyRadius - diff);

    return 1.0;
}

void calcTBN()
{
    vec4 light1_ws = vec4(uLightPos, 1.0f);
    vec3 pos_cs = (uVMatrix * vec4(vPosition, 1.0f)).xyz;

    vec3 camera_cs = (uVMatrix * vec4(uCamPos, 1.0)).xyz;
    vec3 eyeDir_cs = pos_cs - camera_cs;

    tangent_cs = normalize(mat3(uVMatrix) * vTangent);
    bitangent_cs = normalize(mat3(uVMatrix) * vBitangent);
    normal_cs = normalize(mat3(uVMatrix) * vNormal);

    vec3 light1_cs = (uVMatrix * light1_ws).xyz;
    vec3 lightDir1_cs = light1_cs - pos_cs;

    mat3 TBN = transpose(mat3(
                tangent_cs,
                bitangent_cs,
                normal_cs
                ));

    fLightDir1_ts = TBN * lightDir1_cs;
    fEyeDir_ts = TBN * eyeDir_cs;
    test = vec4(pos_cs, 1.0);
}

void main()
{

    float x         = (5.0 + vPosition.x) / 10.0;
    float z         = (5.0 + vPosition.z) / 10.0;

    //float height    = texture2D(heightMap, vec2(x, y)).r;
    float vHeight = vPosition.y;

    //float height = 0.5;
    //fColor = vec3(height, height, height);

    if (vHeight < 0.1) vHeight = 0.09;
    vec4 pos = uVPMatrix * vec4(vPosition.x, vHeight, vPosition.z, 1.0);

    fUV = vUV;
    fNormal = vNormal;
    //fHeight = getHightScale() * vPosition.y;
    fHeight = vPosition.y;

    fPos = vec3(pos);
    wPos = vPosition;
    //calcTBN();













    vec4 light1_ws = vec4(uLightPos, 1.0f);
    vec4 camera_ws = vec4(uCamPos, 1.0f);
    vec3 pos_cs = (uVMatrix * vec4(vPosition, 1.0f)).xyz;

    vec3 camera_cs = (uVMatrix * vec4(uCamPos, 1.0)).xyz;
    vec3 eyeDir_cs = camera_cs - pos_cs;

    tangent_cs = normalize(mat3(uVMatrix) * vTangent);
    bitangent_cs = normalize(mat3(uVMatrix) * vBitangent);
    normal_cs = normalize(mat3(uVMatrix) * vNormal);

    vec3 light1_cs = (uVMatrix * light1_ws).xyz;
    vec3 lightDir1_cs = light1_cs - pos_cs;

    mat3 TBN = transpose(mat3(
                tangent_cs,
                bitangent_cs,
                normal_cs
                ));

    fLightDir1_ts = TBN * lightDir1_cs;
    fEyeDir_ts = TBN * eyeDir_cs;





















    gl_Position = pos;

    //fColor = vec3(y, y, y);
    //fColor = vec3(calculateUV(), height);
    //fColor = vec3(calculateUV(), 0.0);
    //fColor = vec3(0.0, 0.0, texture(heightMap, calculateUV()).r);
    //pos.y = texture(heightMap, vec2(0.5, 0.7)).r;
    //gl_Position = uVPMatrix * vec4(vPosition, 0.0, 1.0);
    //gl_Position = uVPMatrix * vec4(vPosition.x, 0.0, -vPosition.y, 1.0);
}

