#version 410 core
out vec4 color;

void main()
{
 	//color = vec4(texture(heightMap, fUV).r, texture(heightMap, fUV).r, texture(heightMap, fUV).r, 1.0);
 	color = vec4(0.0, 1.0, 1.0, 1.0);
}
