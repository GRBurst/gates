#version 410 core
layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec3 vNormal;
layout (location = 2) in vec3 vTangent;
layout (location = 3) in vec3 vBitangent;
layout (location = 4) in vec2 vUV;

out vec3 wPos;
out vec4 fPos;
out vec2 fUV;
out vec3 fNormal;
out mat3 fTBN;

uniform mat4 uVMatrix;
uniform mat4 uVPMatrix;
uniform int uHeightMapTerrainRatio;
/*
mat3 calcTBN()
{

//    tangent_ws      = normalize(vTangent);
//    bitangent_ws    = normalize(vBitangent);
//    normal_ws       = normalize(vNormal);

//    mat3 TBN_ts_to_ws = mat3(
//                tangent_ws,
//                bitangent_ws,
//                normal_ws
//                );
    vec3 tangent_cs = normalize(mat3(uVMatrix) * vTangent);
    vec3 bitangent_cs = normalize(mat3(uVMatrix) * vBitangent);
    vec3 normal_cs = normalize(mat3(uVMatrix) * vNormal);
    fNormal = normal_cs;

    return transpose(mat3(
                tangent_cs,
                bitangent_cs,
                normal_cs
                ));
}
*/

void main()
{
    wPos = vPosition;
    fPos = uVMatrix * vec4(vPosition, 1.0);
    fUV = vUV;
    fNormal = vNormal;
    //fNormal = mat3(uVMatrix) * vNormal;
    gl_Position = uVPMatrix * vec4(vPosition, 1.0);
    //gl_Position = uVMatrix * vec4(vPosition, 1.0);

    fTBN = mat3(1.0);
    //fTBN = calcTBN();
}
