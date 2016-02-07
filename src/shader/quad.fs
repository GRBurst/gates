#version 410 core
out vec4 color;
in vec2 fUV;

uniform sampler2D sTexture;
void main()
{
    color = vec4(texture(sTexture, fUV).rgb, 1.0);
}
