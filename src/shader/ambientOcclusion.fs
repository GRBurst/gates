#version 410 core
in vec2 fUV;

uniform sampler2D uGPosition;
uniform sampler2D uGNormal;
uniform sampler2D uNoiseSamples;
/* uniform vec3 probes[64]; */
uniform int kernelSize = 64;
uniform vec3 uProbes[64];

layout(location=0) out float color;

// parameters (you'd probably want to use them as uniforms to more easily tweak the effect)
float radius = 1.0;

// tile noise texture over screen based on screen dimensions divided by noise size
const vec2 noiseScale = vec2(1280.0f/4.0f, 1024.0f/4.0f); 

uniform mat4 uPMatrix;

void main()
{

    vec3 screenPos  = texture(uGPosition, fUV).xyz;
    vec3 normal     = texture(uGNormal, fUV).rgb;
    vec3 randVec    = texture(uNoiseSamples, fUV * noiseScale).xyz;

    // Compute tangents
    vec3 tangent    = normalize(randVec - normal * dot(randVec, normal));
    vec3 bitangent  = cross(normal, tangent);
    mat3 TBN        = mat3(tangent, bitangent, normal);

    // Compute occlusion factor
    float probeDepth, rangeCheck, occlusion = 0.0;
    vec3 probe;
    vec4 offset;
    // Sample in hemisphere
    for(int i = 0; i < kernelSize; i++)
    {
        probe = TBN * uProbes[i];
        probe = screenPos + probe * radius; 

        // project probe position (to probe texture) (to get position on screen/texture)
        offset = uPMatrix * vec4(probe, 1.0);
        offset.xy /= offset.w;
        offset.xy = offset.xy * 0.5 + 0.5;

        // get probe depth
        probeDepth = -texture(uGPosition, offset.xy).w; // Get depth value of kernel probe

        // range check & accumulate
        rangeCheck = smoothstep(0.0, 1.0, radius / abs(screenPos.z - probeDepth ));
        occlusion += (probeDepth >= probe.z ? 1.0 : 0.0) * rangeCheck;
        /* occlusion += step(probeDepth, probe.z) * rangeCheck; */
    }
    occlusion = 1.0 - (occlusion / kernelSize);

    /* color = occlusion; */
    color = occlusion*occlusion;
    /* color = 0.5*occlusion; */
}
