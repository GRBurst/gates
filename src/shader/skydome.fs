#version 410 core
out vec4 color;
in vec2 fUV;
uniform sampler2D textureSky; 

void main()
{
 	color = mix(vec4(texture(textureSky, fUV).r, texture(textureSky, fUV).r, texture(textureSky, fUV).r, 1.0), vec4(0.1, 0.22, 0.6, 1.0), 0.4 );
 	//color = vec4(0.0, 1.0, 1.0, 1.0);
}
