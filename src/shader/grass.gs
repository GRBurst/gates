#version 410 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 72) out;


uniform mat4 gVP;
uniform float uTime;
//uniform float uScale;

out float height;
out vec2 texCoord;
out vec3 vColor;
out vec3 vNormal;
out vec3 vPosition;
vec2 mod289(vec2 x)
{
    return x - floor(x * (1.0 / 289.0)) * 289.0;
}

float rand(vec2 co){
    return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}
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
	float fWindStrength = 0.025f;
	vec3 vWindDirection = vec3(1.0, 0.0, 1.0);
	vWindDirection = normalize(vWindDirection);
	float rnd = rand(mod289(gl_in[0].gl_Position.xz));
	float rnd1 = rand(mod289(gl_in[1].gl_Position.xz));
	float rnd2 = rand(mod289(gl_in[2].gl_Position.xz));
	float heightDiff = 0.0;//abs(gl_in[0].gl_Position.y - gl_in[1].gl_Position.y);
	if(gl_in[0].gl_Position.y/3.8 > 0.06 && gl_in[0].gl_Position.y/3.8 < 0.2)
  	{
		float rnd = rand(mod289(gl_in[0].gl_Position.xz));
		float rnd1 = rand(mod289(gl_in[1].gl_Position.xz));
		float rnd2 = rand(mod289(gl_in[2].gl_Position.xz));  		 	
  		vec3 vBaseDirRotated = (rotationMatrix(vec3(0, 1, 0), sin(uTime*0.7f)*0.02f) * vec4(0.0, 1.0, 0.0, 1.0)).xyz;
		vec3 pos = gl_in[0].gl_Position.xyz;		
  		for(int i = 0; i < 3; i++){
  		
  			mat4 rot = rotationMatrix(vec3(0,1,0), 1.0471975512 * i);
  			vec3 posBL = vec3(rot * vec4(-0.05, 0, 0, 1));
			vec3 posBR =  vec3(rot * vec4(0.05, 0, 0, 1));
  			posBL += pos;
			posBR += pos;
			float fWindPower = 0.5f+sin(posBL.x/30+posBL.z/30+uTime*(1.2f+fWindStrength/20.0f));
			if(fWindPower < 0.0f)
				fWindPower = fWindPower*0.2f;
			else fWindPower = fWindPower*0.4f;
	  		fWindPower *= fWindStrength;
	
	  		//BL
			posBL.y += heightDiff;
	  		//gl_Position = gVP * vec4(pos + vec3(0.01, 0.0, 0.01), 1.0);
			gl_Position = gVP * vec4(posBL, 1.0);
			texCoord = vec2(0.0, 0.0);
			height = posBL.y; 
			vec3 posTL = posBL + vWindDirection*fWindPower + vBaseDirRotated * 0.01f;
	 		posTL.y = posBL.y + 0.1;
	 		
	 		vPosition = vec3(gl_Position);
	 		EmitVertex();
	  		//TL
	  		gl_Position = gVP * vec4(posTL, 1.0);
			texCoord = vec2(0.0, 1.0);
			height = posBL.y;
			vPosition = vec3(gl_Position);
			EmitVertex();
			//BR
			posBR.y += heightDiff;
			vec3 posTR = posBR + vWindDirection*fWindPower + vBaseDirRotated * 0.01f;
			//gl_Position = gVP * vec4(pos2 + vec3(0.01, 0.0, 0.01), 1.0);
			texCoord = vec2(1.0, 0.0);
			height = posBR.y;
		    	
		    posTR.y = posBR.y + 0.1;
	  		gl_Position = gVP * vec4(posBR, 1.0);
	  			  
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
	    pos = gl_in[1].gl_Position.xyz;
	    for(int i = 0; i < 3; i++){
  		
  			mat4 rot = rotationMatrix(vec3(0,1,0), 1.0471975512 * i);
  			vec3 posBL = vec3(rot * vec4(-0.05, 0, 0, 1));
			vec3 posBR =  vec3(rot * vec4(0.05, 0, 0, 1));
  			posBL += pos ;
			posBR += pos ;
			float fWindPower = 0.5f+sin(posBL.x/30+posBL.z/30+uTime*(1.2f+fWindStrength/20.0f));
			if(fWindPower < 0.0f)
				fWindPower = fWindPower*0.2f;
			else fWindPower = fWindPower*0.4f;
	  		fWindPower *= fWindStrength;
	
	  		//BL
			posBL.y += heightDiff;
	  		//gl_Position = gVP * vec4(pos + vec3(0.01, 0.0, 0.01), 1.0);
			gl_Position = gVP * vec4(posBL, 1.0);
			texCoord = vec2(0.0, 0.0);
			height = posBL.y; 
			vec3 posTL = posBL + vWindDirection*fWindPower + vBaseDirRotated * 0.01f;
	 		posTL.y = posBL.y + 0.1;
	 		
	 		vPosition = vec3(gl_Position);
	 		EmitVertex();
	  		//TL
	  		gl_Position = gVP * vec4(posTL, 1.0);
			texCoord = vec2(0.0, 1.0);
			height = posBL.y;
			vPosition = vec3(gl_Position);
			EmitVertex();
			//BR
			posBR.y += heightDiff;
			vec3 posTR = posBR + vWindDirection*fWindPower + vBaseDirRotated * 0.01f;
			//gl_Position = gVP * vec4(pos2 + vec3(0.01, 0.0, 0.01), 1.0);
			texCoord = vec2(1.0, 0.0);
			height = posBR.y;
		    	
		    posTR.y = posBR.y + 0.1;
	  		gl_Position = gVP * vec4(posBR, 1.0);
	  			  
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
	    pos = gl_in[2].gl_Position.xyz;
	    for(int i = 0; i < 3; i++){
  		
  			mat4 rot = rotationMatrix(vec3(0,1,0), 1.0471975512 * i);
  			vec3 posBL = vec3(rot * vec4(-0.05, 0, 0, 1));
			vec3 posBR =  vec3(rot * vec4(0.05, 0, 0, 1));
  			posBL += pos ;
			posBR += pos ;
			float fWindPower = 0.5f+sin(posBL.x/30+posBL.z/30+uTime*(1.2f+fWindStrength/20.0f));
			if(fWindPower < 0.0f)
				fWindPower = fWindPower*0.2f;
			else fWindPower = fWindPower*0.4f;
	  		fWindPower *= fWindStrength;
	
	  		//BL
			posBL.y += heightDiff;
	  		//gl_Position = gVP * vec4(pos + vec3(0.01, 0.0, 0.01), 1.0);
			gl_Position = gVP * vec4(posBL, 1.0);
			texCoord = vec2(0.0, 0.0);
			height = posBL.y; 
			vec3 posTL = posBL + vWindDirection*fWindPower + vBaseDirRotated * 0.01f;
	 		posTL.y = posBL.y + 0.1;
	 		
	 		vPosition = vec3(gl_Position);
	 		EmitVertex();
	  		//TL
	  		gl_Position = gVP * vec4(posTL, 1.0);
			texCoord = vec2(0.0, 1.0);
			height = posBL.y;
			vPosition = vec3(gl_Position);
			EmitVertex();
			//BR
			posBR.y += heightDiff;
			vec3 posTR = posBR + vWindDirection*fWindPower + vBaseDirRotated * 0.01f;
			//gl_Position = gVP * vec4(pos2 + vec3(0.01, 0.0, 0.01), 1.0);
			texCoord = vec2(1.0, 0.0);
			height = posBR.y;
		    	
		    posTR.y = posBR.y + 0.1;
	  		gl_Position = gVP * vec4(posBR, 1.0);
	  			  
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

		pos = rnd*((gl_in[2].gl_Position.xyz - gl_in[1].gl_Position.xyz) + (gl_in[0].gl_Position.xyz - gl_in[1].gl_Position.xyz)) / 2  + gl_in[1].gl_Position.xyz;
	    for(int i = 0; i < 3; i++){
  		
  			mat4 rot = rotationMatrix(vec3(0,1,0), 1.0471975512 * i);
  			vec3 posBL = vec3(rot * vec4(-0.05, 0, 0, 1));
			vec3 posBR =  vec3(rot * vec4(0.05, 0, 0, 1));
  			posBL += pos ;
			posBR += pos ;
			float fWindPower = 0.5f+sin(posBL.x/30+posBL.z/30+uTime*(1.2f+fWindStrength/20.0f));
			if(fWindPower < 0.0f)
				fWindPower = fWindPower*0.2f;
			else fWindPower = fWindPower*0.4f;
	  		fWindPower *= fWindStrength;
	
	  		//BL
			posBL.y += heightDiff;
	  		//gl_Position = gVP * vec4(pos + vec3(0.01, 0.0, 0.01), 1.0);
			gl_Position = gVP * vec4(posBL, 1.0);
			texCoord = vec2(0.0, 0.0);
			height = posBL.y; 
			vec3 posTL = posBL + vWindDirection*fWindPower + vBaseDirRotated * 0.01f;
	 		posTL.y = posBL.y + 0.1;
	 		
	 		vPosition = vec3(gl_Position);
	 		EmitVertex();
	  		//TL
	  		gl_Position = gVP * vec4(posTL, 1.0);
			texCoord = vec2(0.0, 1.0);
			height = posBL.y;
			vPosition = vec3(gl_Position);
			EmitVertex();
			//BR
			posBR.y += heightDiff;
			vec3 posTR = posBR + vWindDirection*fWindPower + vBaseDirRotated * 0.01f;
			//gl_Position = gVP * vec4(pos2 + vec3(0.01, 0.0, 0.01), 1.0);
			texCoord = vec2(1.0, 0.0);
			height = posBR.y;
		    	
		    posTR.y = posBR.y + 0.1;
	  		gl_Position = gVP * vec4(posBR, 1.0);
	  			  
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
	    pos = rnd2*(gl_in[2].gl_Position.xyz - gl_in[1].gl_Position.xyz) / 2  + gl_in[1].gl_Position.xyz;
	    for(int i = 0; i < 3; i++){
  		
  			mat4 rot = rotationMatrix(vec3(0,1,0), 1.0471975512 * i);
  			vec3 posBL = vec3(rot * vec4(-0.05, 0, 0, 1));
			vec3 posBR =  vec3(rot * vec4(0.05, 0, 0, 1));
  			posBL += pos ;
			posBR += pos ;
			float fWindPower = 0.5f+sin(posBL.x/30+posBL.z/30+uTime*(1.2f+fWindStrength/20.0f));
			if(fWindPower < 0.0f)
				fWindPower = fWindPower*0.2f;
			else fWindPower = fWindPower*0.4f;
	  		fWindPower *= fWindStrength;
	
	  		//BL
			posBL.y += heightDiff;
	  		//gl_Position = gVP * vec4(pos + vec3(0.01, 0.0, 0.01), 1.0);
			gl_Position = gVP * vec4(posBL, 1.0);
			texCoord = vec2(0.0, 0.0);
			height = posBL.y; 
			vec3 posTL = posBL + vWindDirection*fWindPower + vBaseDirRotated * 0.01f;
	 		posTL.y = posBL.y + 0.1  + 0.025 * rnd;
	 		
	 		vPosition = vec3(gl_Position);
	 		EmitVertex();
	  		//TL
	  		gl_Position = gVP * vec4(posTL, 1.0);
			texCoord = vec2(0.0, 1.0);
			height = posBL.y;
			vPosition = vec3(gl_Position);
			EmitVertex();
			//BR
			posBR.y += heightDiff;
			vec3 posTR = posBR + vWindDirection*fWindPower + vBaseDirRotated * 0.01f;
			//gl_Position = gVP * vec4(pos2 + vec3(0.01, 0.0, 0.01), 1.0);
			texCoord = vec2(1.0, 0.0);
			height = posBR.y;
		    	
		    posTR.y = posBR.y + 0.1 + 0.025*rnd;
	  		gl_Position = gVP * vec4(posBR, 1.0);
	  			  
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
	    
	    pos = rnd1*(gl_in[0].gl_Position.xyz - gl_in[1].gl_Position.xyz) / 2  + gl_in[1].gl_Position.xyz;
	    for(int i = 0; i < 3; i++){
  		
  			mat4 rot = rotationMatrix(vec3(0,1,0), 1.0471975512 * i);
  			vec3 posBL = vec3(rot * vec4(-0.05, 0, 0, 1));
			vec3 posBR =  vec3(rot * vec4(0.05, 0, 0, 1));
  			posBL += pos ;
			posBR += pos ;
			float fWindPower = 0.5f+sin(posBL.x/30+posBL.z/30+uTime*(1.2f+fWindStrength/20.0f));
			if(fWindPower < 0.0f)
				fWindPower = fWindPower*0.2f;
			else fWindPower = fWindPower*0.4f;
	  		fWindPower *= fWindStrength;
	
	  		//BL
			posBL.y += heightDiff;
	  		//gl_Position = gVP * vec4(pos + vec3(0.01, 0.0, 0.01), 1.0);
			gl_Position = gVP * vec4(posBL, 1.0);
			texCoord = vec2(0.0, 0.0);
			height = posBL.y; 
			vec3 posTL = posBL + vWindDirection*fWindPower + vBaseDirRotated * 0.01f;
	 		posTL.y = posBL.y + 0.1;
	 		
	 		vPosition = vec3(gl_Position);
	 		EmitVertex();
	  		//TL
	  		gl_Position = gVP * vec4(posTL, 1.0);
			texCoord = vec2(0.0, 1.0);
			height = posBL.y;
			vPosition = vec3(gl_Position);
			EmitVertex();
			//BR
			posBR.y += heightDiff;
			vec3 posTR = posBR + vWindDirection*fWindPower + vBaseDirRotated * 0.01f;
			//gl_Position = gVP * vec4(pos2 + vec3(0.01, 0.0, 0.01), 1.0);
			texCoord = vec2(1.0, 0.0);
			height = posBR.y;
		    	
		    posTR.y = posBR.y + 0.1;
	  		gl_Position = gVP * vec4(posBR, 1.0);
	  			  
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
