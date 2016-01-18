#version 410 core
layout (location = 0) in vec2 vPosition;
//in vec2 vUV;

uniform mat4 VPMatrix;
uniform vec2 terrainSize;
uniform sampler2D heightMap;

out vec3 vColor;
out vec2 fUV;

vec2 calculateUV()
{
    //return vec2(vPosition.x / terrainSize.x, vPosition.y / terrainSize.y);
    return vec2(0.0 + vPosition.x / 10.0, 0.0 + vPosition.y / 10.0);
    //return vec2(0.2, 0.1);
}

void main()
{
    //vColor = vec3(0.5 + sin(vPosition.x*vPosition.y)/2.0 , 0.5 + cos(vPosition.x*vPosition.y)/2.0, 1.0);


    vec4 pos = VPMatrix * vec4(vPosition.x, texture2D(heightMap, calculateUV()).r, -vPosition.y, 1.0);

    fUV = calculateUV();
    //vColor = vec3(calculateUV(), texture2D(heightMap, calculateUV()).r);
    vColor = vec3(calculateUV(), 0.0);
    //vColor = vec3(0.0, 0.0, texture(heightMap, calculateUV()).r);
    //pos.y = texture(heightMap, vec2(0.5, 0.7)).r;





    gl_Position = pos;

    //gl_Position = VPMatrix * vec4(vPosition, 0.0, 1.0);
    //gl_Position = VPMatrix * vec4(vPosition.x, 0.0, -vPosition.y, 1.0);
}

