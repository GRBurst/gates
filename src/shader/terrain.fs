#version 410 core
in vec3 vColor;
in vec2 fUV;

/* layout(binding = 0) uniform sampler2D heightMap; */
uniform sampler2D heightMap; 

out vec4 color;

void main()
{
    //color = vec4(texture(heightMap, fUV).r, texture(heightMap, fUV).r, texture(heightMap, fUV).r, 1.0);
    //color = vec4(fUV, 0.0, 1.0);
    //color = vec4(0.0, 0.0, 1.0, 1.0);
    color = vec4(vColor, 1.0);
}
