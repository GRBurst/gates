#version 410 core
out vec4 color;
in vec2 fUV;

uniform sampler2D uGPosition;
uniform sampler2D uGNormal;
uniform sampler2D uGAlbedo;
uniform sampler2D uAmbientBlur;

struct Light {
    vec3 Position;
    vec3 Color;

    float Linear;
    float Quadratic;
    float Radius;
};
const int NR_LIGHTS = 32;
uniform Light lights[NR_LIGHTS];
uniform vec3 uCamPos;

uniform int draw_mode;

void main()
{
    // Retrieve data from gbuffer
    vec3 screenPos  = texture(uGPosition, fUV).rgb;
    vec3 normal     = texture(uGNormal, fUV).rgb;
    float depth     = texture(uGPosition, fUV).a;

    vec3 diffLight          = texture(uGAlbedo, fUV).rgb;
    float occlusionFactor   = texture(uAmbientBlur, fUV).r;
    float specularFactor    = 0.3;//texture(gAlbedoSpec, fUV).a;

    // Then calculate lighting as usual
    vec3 resColor   = vec3(occlusionFactor * 0.3);
    vec3 viewDir    = normalize(uCamPos-screenPos);

    // Compute lights
    for(int i = 0; i < NR_LIGHTS; ++i)
    {
        float distance = length(lights[i].Position - screenPos);

        if(distance < lights[i].Radius)
        {
            // diffuse
            vec3 lightDir   = normalize(lights[i].Position - screenPos);
            diffLight       = max(dot(normal, lightDir), 0.0) * diffLight * lights[i].Color;

            // specular
            vec3 halfwayDir = normalize(lightDir + viewDir);
            float spec      = pow(max(dot(normal, halfwayDir), 0.0), 16.0);
            vec3 specular   = lights[i].Color * spec * specularFactor;

            // attenuation
            float attenuation = 1.0 / (1.0 + lights[i].Linear * distance + lights[i].Quadratic * distance * distance);

            // Resulting light / color
            diffLight   *= attenuation;
            specular    *= attenuation;
            resColor    += diffLight + specular;

        }
    }

    color = vec4(resColor, 1.0);
    /* color = vec4(screenPos, 1.0); */
    /* color = vec4(normal, 1.0); */
    /* color = vec4(diffLight, 1.0); */
    /* color = vec4(vec3(occlusionFactor), 1.0); */
}
