#version 410 core
out vec4 color;
in vec2 fUV;

uniform sampler2D sTexture;
uniform int uDim;
void main()
{
    vec3 rgb = texture(sTexture, fUV).rgb;

    if(uDim == 1)
        color = vec4(rgb.r, rgb.r, rgb.r, 1.0);
    else
        color = vec4(rgb, 1.0);
    
}
