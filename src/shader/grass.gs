#version 410 core

layout ( triangles) in;
layout (triangle_strip, max_vertices = 4) out;


uniform mat4 gVP;
uniform vec3 gCameraPos;

out vec2 texCoord;
out vec3 vcolor;
void main()
{
	
	vec3 pos = vec3(gl_in[0].gl_Position.x, gl_in[0].gl_Position.y, gl_in[0].gl_Position.z);
	vec3 pos2 = vec3(gl_in[1].gl_Position.x, gl_in[1].gl_Position.y, gl_in[1].gl_Position.z);
	
	float heightDiff = abs(pos2.y - pos.y);
	if(abs(pos.x- pos2.x) < 1)
	  	if(pos.y > 0.2 && pos.y < 0.6)
	  	{
	  		pos.y += 0.15 * heightDiff;
		    gl_Position = gVP * vec4(pos, 1.0);
		    texCoord = vec2(0.0, 0.0);
		    EmitVertex();
		 
		 	pos.y += 0.05;
		    gl_Position = gVP * vec4(pos, 1.0);
		    texCoord = vec2(0.0, 1.0);
		    EmitVertex();
		
			pos = pos2;
			pos.y += 0.1 * heightDiff;
		    gl_Position = gVP * vec4(pos, 1.0);
		    texCoord = vec2(1.0, 0.0);
		    EmitVertex();
		
		    pos.y += 0.05;
		    gl_Position = gVP * vec4(pos, 1.0);
		    texCoord = vec2(1.0, 1.0);
		    EmitVertex();
		    EndPrimitive();
	    }
}