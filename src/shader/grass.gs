#version 410 core

layout ( triangles) in;
layout (triangle_strip, max_vertices = 4) out;


uniform mat4 gVP;
uniform vec3 gCameraPos;
uniform float uTime;
//uniform float uScale;

out float height;
out vec2 texCoord;
out vec3 vColor;

mat4 rotationMatrix(vec3 axis, float angle){
	axis = normalize(axis);
	float sinVal = sin(angle);
	float cosVal = cos(angle);
	float cosToOne = 1.0f -cosVal;
	
	return mat4(cosVal + axis.x * axis.x * cosToOne,		axis.x * axis.y * cosToOne - axis.z * sinVal, 		axis.z * axis.x * cosToOne + axis.y * sinVal, 	0.0f,
				axis.x * axis.y * cosToOne + axis.z*sinVal,	cosVal + axis.y * axis.y * cosToOne,		 		axis.y * axis.z * cosToOne - axis.x * sinVal, 	0.0f,
				axis.z * axis.x * cosToOne - axis.y*sinVal,	axis.z * axis.y * cosToOne + axis.x * sinVal, 		cosVal + axis.z * axis.z  * cosToOne,	 		0.0f,
				0.0f,										0.0f,												0.0f,											1.0f);
}

void main()
{
	float fWindStrength = 0.15f;
	vec3 vWindDirection = vec3(1.0, 0.0, 1.0);
	vWindDirection = normalize(vWindDirection);
	
	vec3 pos = vec3(gl_in[0].gl_Position.x, gl_in[0].gl_Position.y, gl_in[0].gl_Position.z);
	vec3 pos2 = vec3(gl_in[1].gl_Position.x, gl_in[1].gl_Position.y, gl_in[1].gl_Position.z);
	vec3 center = pos2 - pos;
	float heightDiff = abs(pos2.y - pos.y);
	if(abs(pos.x- pos2.x) < 1){
		
	  	if(pos.y > 0.2 && pos.y < 0.6)
	  	{
	  		vec3 vBaseDirRotated = (rotationMatrix(vec3(0, 1, 0), sin(uTime*0.7f)*0.1f) * vec4(0.0, 1.0, 0.0, 1.0)).xyz;
		  	float fWindPower = 0.5f+sin(center.x/30+center.z/30+uTime*(1.2f+fWindStrength/20.0f));
			if(fWindPower < 0.0f)
				fWindPower = fWindPower*0.2f;
			else fWindPower = fWindPower*0.3f;
	  		fWindPower *= fWindStrength;
	  		
	  		 pos.y += 0.3 * heightDiff;
	  		//gl_Position = gVP * vec4(pos + vec3(0.01, 0.0, 0.01), 1.0);
		    gl_Position = gVP * vec4(pos, 1.0);
		    texCoord = vec2(0.0, 0.0);
		    height = pos.y; 
		    EmitVertex();
	  		
	  		pos.y += 0.05;
		    gl_Position = gVP * vec4(pos + vWindDirection*fWindPower + vBaseDirRotated * 0.01f, 1.0);
		    texCoord = vec2(0.0, 1.0);
		    height = pos.y;
		    EmitVertex();
			
			pos2.y += 0.3 * heightDiff;
		 	//gl_Position = gVP * vec4(pos2 + vec3(0.01, 0.0, 0.01), 1.0);
		    gl_Position = gVP * vec4(pos2, 1.0);
		    texCoord = vec2(1.0, 0.0);
		    height = pos.y;
		    EmitVertex();
			
			
			pos2.y += 0.05;
		    gl_Position = gVP * vec4(pos2+ vWindDirection*fWindPower + vBaseDirRotated * 0.01f, 1.0);
		    texCoord = vec2(1.0, 1.0);
		    height = pos.y;
		    EmitVertex();
	 
		    EndPrimitive();
	    }
    }
}