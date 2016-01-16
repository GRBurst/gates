#version 410 core

layout (points) in;
layout (triangle_strip) out;
layout (max_vertices = 4) out;

uniform mat4 mvp;
uniform vec3 cameraPos;

out vec2 texCoord;

void main(){
    vec3 pos = gl_in[0].gl_Position.xyz;
    pos /= gl_in[0].gl_Position.w; //normalized device coordinates
    vec3 toCamera = normalize(cameraPos - pos);
    vec3 up = vec3(0,1,0);
    vec3 right = normalize(cross(up, toCamera)); //right-handed coordinate system
    //vec3 right = cross(toCamera, up); //left-handed coordinate system

    pos -= (right*0.5);
    gl_Position = mvp*vec4(pos,1.0);
    texCoord = vec2(0,0);
    EmitVertex();

    pos.y += 1.0;   
    gl_Position = mvp*vec4(pos,1.0);
    texCoord = vec2(0,1);
    EmitVertex();

    pos.y -= 1.0;   
    pos += right;
    gl_Position = mvp*vec4(pos,1.0);
    texCoord = vec2(1,0);
    EmitVertex();

    pos.y += 1.0;       
    gl_Position = mvp*vec4(pos,1.0);
    texCoord = vec2(1,1);
    EmitVertex();
}
