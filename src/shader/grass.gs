#version 410 core

layout (points) in;
layout (triangle_strip, max_vertices = 4) out;


uniform mat4 gVP;
uniform vec3 gCameraPos;

out vec2 texCoord;
out vec3 vcolor;
void main()
{
	
	vec3 pos = vec3(gl_in[0].gl_Position.x, gl_in[0].gl_Position.y, -gl_in[0].gl_Position.z);
  	
 	vec3 toCamera = vec3(0.0, 0.0, -1.0);//normalize(gCameraPos - pos);
    vec3 up = vec3(0.0, 1.0, 0.0);
    vec3 right = cross(toCamera, up);
 
    pos -= (right * 0.01);
    gl_Position = gVP * vec4(pos, 1.0);
    texCoord = vec2(0.0, 0.0);
    EmitVertex();
 
    pos.y += 0.02;
    gl_Position = gVP * vec4(pos, 1.0);
    texCoord = vec2(0.0, 1.0);
    EmitVertex();

    pos.y -= 0.02;
    pos += right * 0.02;
    gl_Position = gVP * vec4(pos, 1.0);
    texCoord = vec2(1.0, 0.0);
    EmitVertex();

    pos.y += 0.02;
    gl_Position = gVP * vec4(pos, 1.0);
    texCoord = vec2(1.0, 1.0);
    EmitVertex();
    EndPrimitive();
}