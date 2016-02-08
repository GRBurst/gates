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
    vec3 screenPos_cs  = texture(uGPosition, fUV).rgb;     // camera coords
    vec3 normal     = texture(uGNormal, fUV).rgb;       // world space?!
    float depth     = texture(uGPosition, fUV).a;       // depth

    vec3 diffBase          = texture(uGAlbedo, fUV).rgb;
    float occlusionFactor   = texture(uAmbientBlur, fUV).r;
    float specularFactor    = 0.3;//texture(gAlbedoSpec, fUV).a;

    // Then calculate lighting as usual
    /* vec3 resColor   = vec3(occlusionFactor * 0.3); */
    /* vec3 resColor   = occlusionFactor * diffLight; */
    vec3 diffLight   = occlusionFactor * diffBase;
    vec3 viewDir    = normalize(-screenPos_cs);

    // Compute lights
    for(int i = 0; i < NR_LIGHTS; ++i)
    {
        float distance = length(lights[i].Position - screenPos_cs);

        if(distance < lights[i].Radius)
        {
            // diffuse
            /* vec3 lightDir       = normalize(lights[i].Position - screenPos_cs); */
            /* float intDiff       = max(dot(normalize(normal), lightDir), 0.0); */
            /* diffLight           = intDiff * diffLight;// * lights[i].Color; */
            /* vec3 reflectDir     = reflect(-lightDir, normal); */
            /* float cosAlpha_ts   = clamp( dot( eye_ts, reflection_ts ), 0, 1 ); */
            /* diffLight       =  * diffLight * lights[i].Color; */
            /* diffLight = intDiff * lights[i].Color; */

            // specular
            /* vec3 halfwayDir = normalize(lightDir + viewDir); */
            /* float spec      = pow(max(dot(normal, halfwayDir), 0.0), 16.0); */
            /* vec3 specular   = lights[i].Color * spec * specularFactor; */

            // attenuation
            /* float attenuation = 1.0 / (1.0 + lights[i].Linear * distance + lights[i].Quadratic * distance * distance); */

            // Resulting light / color
            /* diffLight   *= attenuation; */
            /* specular    *= attenuation; */
            /* resColor    += diffLight + specular; */

        }
    }

    /* color = vec4(resColor, 1.0); */
    /* color = vec4(screenPos_cs, 1.0); */
    /* color = vec4(normal, 1.0); */
    /* color = vec4(diffBase, 1.0); */
    color = vec4(diffLight, 1.0);
    /* color = vec4(vec3(depth), 1.0); */
    /* color = vec4(vec3(occlusionFactor), 1.0); */
}
