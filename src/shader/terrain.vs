#version 410 core
layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec3 vNormal;
//in vec2 fUV;

uniform sampler2D heightMap;
uniform mat4 uVPMatrix;
uniform mat3 uInvViewMatrix;
uniform int uHeightMapTerrainRatio;
uniform int uDrawGrid;
uniform vec3 uRayTerrainIntersection;
uniform vec3 uCamPos;
out float fHeight;
out vec2 fUV;
out vec3 fNormal;
out vec3 fPos;
out vec3 wPos;

vec2 calculateUV()
{
    return vec2((25.0 + vPosition.x) / 50.0, (25.0 + vPosition.y) / 50.0);
    //return vec2(0.2, 0.1);
}

float getHightScale()
{

    vec2 ray = vec2(uRayTerrainIntersection.x - vPosition.x, uRayTerrainIntersection.z - vPosition.z);
    float diff = length(ray);
    if(diff < 2.5) return (2.5 - diff);

    return 1.0;
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

    fUV = calculateUV();
    fNormal = vNormal;
    //fHeight = getHightScale() * vPosition.y;
    fHeight = vPosition.y;

    fPos = vec3(pos);
    wPos = vPosition;
    gl_Position = pos;

    //fColor = vec3(y, y, y);
    //fColor = vec3(calculateUV(), height);
    //fColor = vec3(calculateUV(), 0.0);
    //fColor = vec3(0.0, 0.0, texture(heightMap, calculateUV()).r);
    //pos.y = texture(heightMap, vec2(0.5, 0.7)).r;
    //gl_Position = uVPMatrix * vec4(vPosition, 0.0, 1.0);
    //gl_Position = uVPMatrix * vec4(vPosition.x, 0.0, -vPosition.y, 1.0);
}

