#version 410 core
in vec2 fUV;
out float color;

uniform sampler2D sAmbientColor;
uniform int uBlurSize = 4;

void main()
{
    vec2 texelSize = 1.0 / vec2(textureSize(sAmbientColor, 0));

    vec2 offset;
    float result = 0.0;
    int range = uBlurSize / 2;
    for (int x = -range; x < range; x++)
    {
        for (int y = -range; y < range; y++) 
        {
            offset = vec2(float(x), float(y)) * texelSize;
            result += texture(sAmbientColor, fUV + offset).r;
        }
    }
    color = result / float(uBlurSize*uBlurSize);
}
