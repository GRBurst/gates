#version 410 core
layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec3 vNormal;
//in vec2 fUV;

uniform mat4 VPMatrix;
uniform vec2 terrainSize;
uniform sampler2D heightMap;

out vec3 fColor;
out vec2 fUV;
out vec3 fNormal;

vec2 calculateUV()
{
    return vec2((5.0 + vPosition.x) / 10.0, (5.0 + vPosition.y) / 10.0);
    //return vec2(0.2, 0.1);
}

void main()
{
    //fColor = vec3(0.5 + sin(vPosition.x*vPosition.y)/2.0 , 0.5 + cos(vPosition.x*vPosition.y)/2.0, 1.0);

    float x         = (5.0 + vPosition.x) / 10.0;
    float y         = (5.0 + vPosition.y) / 10.0;
    //float height    = texture2D(heightMap, vec2(x, y)).r;
    float height    = vPosition.z;
    //float height = 0.5;
    if (height < 0.2) height = 0.2;
    float hScale    = 1.0;

    vec4 pos = VPMatrix * vec4(vPosition.x, hScale * height, -vPosition.y, 1.0);

    fColor = vec3(height, height, height);
    fUV = calculateUV();
    fNormal = vec3(VPMatrix * vec4(vNormal, 1.0)); 
    gl_Position = pos;

    //fColor = vec3(y, y, y);
    //fColor = vec3(calculateUV(), height);
    //fColor = vec3(calculateUV(), 0.0);
    //fColor = vec3(0.0, 0.0, texture(heightMap, calculateUV()).r);
    //pos.y = texture(heightMap, vec2(0.5, 0.7)).r;
    //gl_Position = VPMatrix * vec4(vPosition, 0.0, 1.0);
    //gl_Position = VPMatrix * vec4(vPosition.x, 0.0, -vPosition.y, 1.0);
}

