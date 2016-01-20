#version 410 core
layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec3 vNormal;
//in vec2 fUV;

uniform sampler2D heightMap;
uniform mat4 uVPMatrix;
uniform mat3 uInvViewMatrix;
uniform int uHeightMapTerrainRatio;
uniform int uDrawGrid;

out vec3 fColor;
out vec2 fUV;
out vec3 fNormal;
out vec3 fPos;

vec2 calculateUV()
{
    return vec2((5.0 + vPosition.x) / 10.0, (5.0 + vPosition.z) / 10.0);
    //return vec2(0.2, 0.1);
}

void main()
{

    float x         = (5.0 + vPosition.x) / 10.0;
    float z         = (5.0 + vPosition.z) / 10.0;
    float hScale    = 1.0;

    //float height    = texture2D(heightMap, vec2(x, y)).r;
    float height    = hScale * vPosition.y;

    //float height = 0.5;
    fColor = vec3(height, height, height);

    if (height < 0.2) height = 0.2;
    vec4 pos = uVPMatrix * vec4(vPosition.x, hScale * height, vPosition.z, 1.0);

    fUV = calculateUV();
    fNormal = uInvViewMatrix * vNormal;
    //fNormal = vNormal;

    fPos = vec3(pos);
    gl_Position = pos;

    //fColor = vec3(y, y, y);
    //fColor = vec3(calculateUV(), height);
    //fColor = vec3(calculateUV(), 0.0);
    //fColor = vec3(0.0, 0.0, texture(heightMap, calculateUV()).r);
    //pos.y = texture(heightMap, vec2(0.5, 0.7)).r;
    //gl_Position = uVPMatrix * vec4(vPosition, 0.0, 1.0);
    //gl_Position = uVPMatrix * vec4(vPosition.x, 0.0, -vPosition.y, 1.0);
}

