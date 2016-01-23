#version 410 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 12) out;


uniform mat4 gVP;
uniform float uTime;
//uniform float uScale;

out float height;
out vec2 texCoord;
out vec3 vColor;
out vec3 vNormal;
out vec3 vPosition;

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
	
	
	vec3 vec01 = gl_in[1].gl_Position.xyz - gl_in[0].gl_Position.xyz;
	vec3 vec02 = gl_in[2].gl_Position.xyz - gl_in[0].gl_Position.xyz;
	vec3 normal = normalize(cross(vec01, vec02));
	vec3 posBL = gl_in[0].gl_Position.xyz;
	vec3 posBR = gl_in[1].gl_Position.xyz;
	float heightDiff = abs(posBR.y - posBL.y);
	if(posBL.y > 0.1 && posBL.y < 0.7)
  	{
  		for(int i = 0; i < gl_in.length(); i++){
  			if(bool(gl_PrimitiveIDIn & 1)){
	  			posBL = gl_in[int(mod(i, 3))].gl_Position.xyz;
				posBR = gl_in[int(mod(i +1, 3))].gl_Position.xyz;
		  		vec3 vBaseDirRotated = (rotationMatrix(vec3(0, 1, 0), sin(uTime*0.7f)*0.1f) * vec4(0.0, 1.0, 0.0, 1.0)).xyz;
			  	float fWindPower = 0.5f+sin(posBL.x/30+posBL.z/30+uTime*(1.2f+fWindStrength/20.0f));
				if(fWindPower < 0.0f)
					fWindPower = fWindPower*0.2f;
				else fWindPower = fWindPower*0.4f;
		  		fWindPower *= fWindStrength;
		
		  		//BL
		  		posBL.y += 0.3 * heightDiff;
		  		//gl_Position = gVP * vec4(pos + vec3(0.01, 0.0, 0.01), 1.0);
			    gl_Position = gVP * vec4(posBL, 1.0);
			    texCoord = vec2(0.0, 0.0);
			    height = posBL.y; 
			    vec3 posTL = posBL + vWindDirection*fWindPower + vBaseDirRotated * 0.01f;
		  		posTL.y = posBL.y + 0.25;
		  		vNormal = posTL - posBL;
		  		vPosition = vec3(gl_Position);
		  		EmitVertex();
		  		//TL
		  		gl_Position = gVP * vec4(posTL, 1.0);
			    texCoord = vec2(0.0, 1.0);
			    height = posBL.y;
			    vPosition = vec3(gl_Position);
			    EmitVertex();
				//BR
				posBR.y += 0.3 * heightDiff;
			    vec3 posTR = posBR + vWindDirection*fWindPower + vBaseDirRotated * 0.01f;
			 	//gl_Position = gVP * vec4(pos2 + vec3(0.01, 0.0, 0.01), 1.0);
			    texCoord = vec2(1.0, 0.0);
			    height = posBR.y;
			    
		  		posTR.y = posBR.y + 0.25;
		  		gl_Position = gVP * vec4(posBR, 1.0);
		  		vNormal = posTR - posBR;	  
		  		vPosition = vec3(gl_Position);		
			    EmitVertex();
				
				//TR
				
			    gl_Position = gVP * vec4(posTR, 1.0);
			    texCoord = vec2(1.0, 1.0);
			    height = posBR.y;
			    vPosition = vec3(gl_Position);
			    EmitVertex();
		 		
			    EndPrimitive();
		    }
	    }

	}
	

}
