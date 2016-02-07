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
    vec2 TexCoords = fUV;
    //vec3 viewPos = uCamPos;
    // Retrieve data from gbuffer
    vec3 FragPos = texture(uGPosition, TexCoords).rgb;
    vec3 Normal = texture(uGNormal, TexCoords).rgb;
    vec3 Diffuse = texture(uGAlbedo, TexCoords).rgb;
    float Depth = texture(uGPosition, TexCoords).a;
    float Specular = 0.3;//texture(gAlbedoSpec, TexCoords).a;
    float AmbientOcclusion = texture(uAmbientBlur, TexCoords).r;

    // Then calculate lighting as usual
    vec3 ambient = vec3(0.3 * AmbientOcclusion);
    /* vec3 lighting  = Diffuse * 0.3; // hard-coded ambient component */
    vec3 lighting  = ambient;
    /* vec3 viewDir  = normalize(viewPos - FragPos); */
    vec3 viewDir  = normalize(-FragPos);

    // Lights
    for(int i = 0; i < NR_LIGHTS; ++i)
    {
        // Calculate distance between light source and current fragment
        float distance = length(lights[i].Position - FragPos);
        if(distance < lights[i].Radius)
        {
            // Diffuse
            vec3 lightDir = normalize(lights[i].Position - FragPos);
            vec3 diffuse = max(dot(Normal, lightDir), 0.0) * Diffuse * lights[i].Color;
            // Specular
            vec3 halfwayDir = normalize(lightDir + viewDir);
            float spec = pow(max(dot(Normal, halfwayDir), 0.0), 16.0);
            /* vec3 specular = lights[i].Color * spec * Specular; */
            vec3 specular = lights[i].Color * spec;
            // Attenuation
            float attenuation = 1.0 / (1.0 + lights[i].Linear * distance + lights[i].Quadratic * distance * distance);
            diffuse *= attenuation;
            specular *= attenuation;
            lighting += diffuse + specular;
        }
    }

    // Based on which of the 1-5 keys we pressed, show final result or intermediate g-buffer textures
    color = vec4(lighting, 1.0);
    /* color = vec4(FragPos, 1.0); */
    /* color = vec4(Normal, 1.0); */
    /* color = vec4(Diffuse, 1.0); */
    /* color = vec4(vec3(Specular), 1.0); */
}
