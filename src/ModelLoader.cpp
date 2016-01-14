#include "ModelLoader.h"
ModelLoader::ModelLoader(const char* filePath, GLint shaderProgram) :
    VBO(0)
    , VAO(0)
    , modelLocation(0)
    , projectionLocation(0)
    , viewLocation(0)
    , vertexCount(0)
    , normalCount(0)
    , uvCount(0)
    , faces(0)
    , lightPosition(vec3(0))
    , cameraPosition(vec3(0))
	, MVP(glm::mat4(1))
{
	fileStream = new ifstream();
	fileStream->open(filePath, ios::in);
	if(!fileStream->is_open()){
		cerr << "Kann nicht geoeffnet werden. " << filePath << " existiert nicht." << endl;
		//delete this;
	}
	projection = mat4(1.0f);
	/* textureCount = 0; */
	this->shaderProgram = shaderProgram;
	cout << "Successfully loaded" << endl;
}

/* ModelLoader::ModelLoader(const char* filePath, const char* imagePath,  GLint shaderProgram) : */
/*     ModelLoader::ModelLoader(filePath, shaderProgram) */
/* { */
/* 	texture = new Texture(imagePath); */
/* 	textureCount = 1; */
/* } */

/* ModelLoader::ModelLoader(const char* filePath, const char* imagePath, const char* normalMapPath, GLint shaderProgram) : */
/*     ModelLoader::ModelLoader(filePath, imagePath, shaderProgram) */
/* { */
/* 	normalmap = new Texture(normalMapPath); */
/* 	textureCount = 2; */
/* } */

/* ModelLoader::ModelLoader(const char* filePath, const char* imagePath, const char* normalMapPath, const char* heightMapPath, GLint shaderProgram) : */
/*     ModelLoader::ModelLoader(filePath, imagePath, normalMapPath, shaderProgram) */
/* { */
/* 	heightmap = new Texture(heightMapPath); */
/* 	textureCount = 3; */
/* } */

ModelLoader::~ModelLoader()
{
	if(fileStream->is_open()){
		fileStream->close();
	}
	delete []vertexBuffer;
	/* if (textureCount >= 1) */
	/* { */
	/* 	delete texture; */
	/* 	if (textureCount >= 2) */
	/* 	{ */
	/* 		delete normalmap; */
	/* 		if (textureCount >= 3) */
	/* 		{ */
	/* 			delete heightmap; */
	/* 		} */
	/* 	} */
	/* } */
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
}

void ModelLoader::loadFile(){
	string line = "";
	while (getline(*fileStream, line)){
		if (line.at(0) == 'v' && line.at(1) == ' '){
			/*
			for (int i = 0; i < 4; i++)
			{
				int spacePos = lineValues.find(' ', oldSpacePos + 1);
				lineFaces.push_back(lineValues.substr(oldSpacePos + 1, spacePos - oldSpacePos));
				oldSpacePos = spacePos;
			}*/
			string lineValues = line.substr(2);
			vec3 vector;

			int spacePos = lineValues.find(' ');
			string value = lineValues.substr(0, spacePos);
			lineValues = lineValues.substr(spacePos + 1);
			vector.x = (float)atof(value.c_str());

			spacePos = lineValues.find(' ');
			value = lineValues.substr(0, spacePos);
			lineValues = lineValues.substr(spacePos + 1);
			vector.y = (float)atof(value.c_str());

			spacePos = lineValues.find(' ');
			value = lineValues.substr(0, spacePos);
			vector.z = (float)atof(value.c_str());
			fileVertices.push_back(vector);

			//cout << vector.x << ", " << vector.y << ", " << vector.z << endl;
		}

		if (line.at(0) == 'v' && line.at(1) == 't' && line.at(2) == ' '){
			string lineValues = line.substr(3);
			vec2 vector;

			int spacePos = lineValues.find(' ');
			string value = lineValues.substr(0, spacePos);
			lineValues = lineValues.substr(spacePos + 1);
			vector.x = (float)atof(value.c_str());

			spacePos = lineValues.find(' ');
			value = lineValues.substr(0, spacePos);
			lineValues = lineValues.substr(spacePos + 1);
			vector.y = (float)atof(value.c_str());

			fileUvs.push_back(vector);
		}

		if (line.at(0) == 'v' && line.at(1) == 'n' && line.at(2) == ' '){
			string lineValues = line.substr(3);
			vec3 vector;

			int spacePos = lineValues.find(' ');
			string value = lineValues.substr(0, spacePos);
			lineValues = lineValues.substr(spacePos + 1);
			vector.x = (float)atof(value.c_str());

			spacePos = lineValues.find(' ');
			value = lineValues.substr(0, spacePos);
			lineValues = lineValues.substr(spacePos + 1);
			vector.y = (float)atof(value.c_str());

			spacePos = lineValues.find(' ');
			value = lineValues.substr(0, spacePos);
			vector.z = (float)atof(value.c_str());

//			fileNormals.push_back(vector);
		}

		if (line.at(0) == 'f' && line.at(1) == ' '){
			string lineValues = line.substr(2);
			vector<string> lineFaces;
			array<string, 3> faceVal;
			int upperBound = 0;
			for(unsigned int i = 0; i < line.size(); i++)
			{
				if (line[i] == ' ')
					upperBound++;
			}

			string vertexIndex, uvIndex, normalIndex;
			for (int i = 0; i < upperBound; i++){
				int valPos = lineValues.find('/');
				vertexIndex = lineValues.substr(0, valPos);
				lineValues = lineValues.substr(valPos + 1);
				valPos = lineValues.find('/');
				uvIndex = lineValues.substr(0, valPos);
				lineValues = lineValues.substr(valPos + 1);
				valPos = lineValues.find(' ');
				normalIndex = lineValues.substr(0, valPos);
				lineValues = lineValues.substr(valPos + 1);

				vertexIndices.push_back(atoi(vertexIndex.c_str()));
				uvIndices.push_back(atoi(uvIndex.c_str()));
//				normalIndices.push_back(atoi(normalIndex.c_str()));
			}
			faces++;

		}
	}
	std::cout << fileUvs.size() << std::endl;
	std::cout << uvIndices.size() << std::endl;
	fileStream->close();
}
vector<int> ModelLoader::getVertexIndices(){
	return vertexIndices;
}

vector<vec3> ModelLoader::getVertices(){

	return fileVertices;
}
void ModelLoader::setStride(){

	if (!this->fileVertices.empty() && !this->fileNormals.empty() && !this->fileUvs.empty()){

		for(unsigned long i = 0; i < vertexIndices.size(); i++){
			//std::cout << "HIERE" << std::endl;
			vertices.push_back(this->fileVertices.at(vertexIndices.at(i)-1));
		}

//		for(unsigned long i = 0; i < normalIndices.size(); i++){
//			//std::cout << "HIERE" << std::endl;
//			normals.push_back(this->fileNormals.at(normalIndices.at(i)-1));
//		}

		for(unsigned long i = 0; i < uvIndices.size(); i++){
			//std::cout << "HIERE" << std::endl;
			uvs.push_back(this->fileUvs.at(uvIndices.at(i)-1));
		}
		GLfloat *saveBuffer;

		this->calculateTangents();

		vertexBuffer = new GLfloat[vertices.size() * 3 + uvs.size()];// * 2 + tangents.size() * 3 + bitangents.size() * 3+ normals.size() * 3 ];
		saveBuffer = vertexBuffer;
		for (unsigned long i = 0; i < vertices.size(); i++){
			*saveBuffer = vertices.at(i).x;
			saveBuffer++;
			*saveBuffer = vertices.at(i).y;
			saveBuffer++;
			*saveBuffer = vertices.at(i).z;
			saveBuffer++;
//			*saveBuffer = normals.at(i).x;
//			saveBuffer++;
//			*saveBuffer = normals.at(i).y;
//			saveBuffer++;
//			*saveBuffer = normals.at(i).z;
//			saveBuffer++;
//			*saveBuffer = tangents.at(i).x;
//			saveBuffer++;
//			*saveBuffer = tangents.at(i).y;
//			saveBuffer++;
//			*saveBuffer = tangents.at(i).z;
//			saveBuffer++;
//			*saveBuffer = bitangents.at(i).x;
//			saveBuffer++;
//			*saveBuffer = bitangents.at(i).y;
//			saveBuffer++;
//			*saveBuffer = bitangents.at(i).z;
//			saveBuffer++;
			*saveBuffer = uvs.at(i).x;
			saveBuffer++;
			*saveBuffer = uvs.at(i).y;
			saveBuffer++;
		}
		vertexCount = vertices.size();
		normalCount = normals.size();
		uvCount = uvs.size();
		std::cout << "HIERE" << std::endl;
	}
}
void ModelLoader::setVerticesAsArray(){
	vector<vec3> out;
	for(unsigned int i = 0; i < vertexIndices.size(); i++){
			out.push_back(fileVertices.at(vertexIndices.at(i)-1));
		}
	GLfloat *saveBuffer;

	vertexBuffer = new GLfloat[fileVertices.size() * 3];
	saveBuffer = vertexBuffer;
	for (unsigned int i = 0; i < fileVertices.size(); i++){
		*saveBuffer = fileVertices.at(i).x;
		saveBuffer++;
		*saveBuffer = fileVertices.at(i).y;
		saveBuffer++;
		*saveBuffer = fileVertices.at(i).z;
		saveBuffer++;
	}

}
void ModelLoader::setBuffers(){
	this->setStride();
	cout << "Buffers for "<< shaderProgram << " set" << endl;
	GLint posAttrib = glGetAttribLocation(shaderProgram, "vPosition");
	/* GLint normAttrib = glGetAttribLocation(shaderProgram, "normal"); */
	GLint uvAttrib = glGetAttribLocation(shaderProgram, "vUV");
	/* GLint tanAttrib = glGetAttribLocation(shaderProgram, "tangent"); */
	/* GLint bitAttrib = glGetAttribLocation(shaderProgram, "bitangent"); */

	//Erzeuge Vertex Array Object und Vertex Buffer Object
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	//Binde VBO an VAO
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, (vertexCount * 14) * sizeof(GLfloat), vertexBuffer, GL_STATIC_DRAW);
	//location, attribute size vec3, data type, bool normalized?, stride size, offset to first dataobject in array
//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
//	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(int), elementBuffer, GL_STATIC_DRAW);

	glEnableVertexAttribArray(posAttrib);
	glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*) 0);
	/* glEnableVertexAttribArray(normAttrib); */
	/* glVertexAttribPointer(normAttrib, 3, GL_FLOAT, GL_TRUE, 14 * sizeof(GLfloat), (GLvoid*) (3 * sizeof(GLfloat))); */
	/* glEnableVertexAttribArray(tanAttrib); */
	/* glVertexAttribPointer(tanAttrib, 3, GL_FLOAT, GL_TRUE, 14 * sizeof(GLfloat), (GLvoid*) (6 * sizeof(GLfloat))); */
	/* glEnableVertexAttribArray(bitAttrib); */
	/* glVertexAttribPointer(bitAttrib, 3, GL_FLOAT, GL_TRUE, 14 * sizeof(GLfloat), (GLvoid*) (9 * sizeof(GLfloat))); */
	glEnableVertexAttribArray(uvAttrib);
	glVertexAttribPointer(uvAttrib, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*) (3 * sizeof(GLfloat)));

	//Unbind VBO
	//Setze Texture in ShaderProgram an TEXTURE0
	/* if (textureCount >= 1) */
	/* { */
	/* 	texture->setTexture(shaderProgram, 0, "mainTexture"); */
	/* } */

	/* //Setze Texture in ShaderProgram an TEXTURE1 */
	/* if (textureCount >= 2) */
	/* { */
	/* 	normalmap->setTexture(shaderProgram, 1, "normalMap"); */
	/* } */
	/* //glBindBuffer(GL_ARRAY_BUFFER, 0); */
	/* if (textureCount >= 3) */
	/* { */
	/* 	heightmap->setTexture(shaderProgram, 2, "heightMap"); */
	/* } */

	//Unbinde VAO
	glBindVertexArray(0);
}

//Besser zu GameObject draw Call bewegen
void ModelLoader::draw(){
	glUseProgram(shaderProgram);
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, value_ptr(model));
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, value_ptr(view));
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, value_ptr(projection));
	glUniformMatrix4fv(MVPLocation, 1, GL_FALSE, value_ptr(MVP));
	glUniformMatrix4fv(MVLocation, 1, GL_FALSE, value_ptr(MV));
	glUniformMatrix3fv(MV3Location, 1, GL_FALSE, value_ptr(MV3));
	glUniform3f(lightLoc, lightPosition.x, lightPosition.y, lightPosition.z);
	glUniform3f(cameraLoc, cameraPosition.x, cameraPosition.y, cameraPosition.z);
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, vertexCount);
	/* if (textureCount >= 1) */
	/* 	texture->bind(); */
	/* if (textureCount >= 2) */
	/* 	normalmap->bind(); */
	/* if (textureCount >= 3) */
	/* 	heightmap->bind(); */
	//cout << shaderProgram << endl;
	glBindVertexArray(0);
	//glUseProgram(0);

}

void ModelLoader::setStandardUniformLocations(){
	cout << "Uniform locations for "<< shaderProgram << " set" << endl;
	modelLocation = glGetUniformLocation(shaderProgram, "M");
	viewLocation = glGetUniformLocation(shaderProgram, "V");
	projectionLocation = glGetUniformLocation(shaderProgram, "P");
	MVPLocation = glGetUniformLocation(shaderProgram, "MVP");
	MVLocation = glGetUniformLocation(shaderProgram, "MV");
	MV3Location = glGetUniformLocation(shaderProgram, "MV3");
	lightLoc = glGetUniformLocation(shaderProgram, "light_position");
	cameraLoc = glGetUniformLocation(shaderProgram, "camera_position");
}

void ModelLoader::changeShader(GLint &shaderProgram){
	this->shaderProgram = shaderProgram;
}

void ModelLoader::setView(mat4 &view){
	this->view = view;
	this->setMVP();
}

void ModelLoader::setProjection(mat4 &projection){
	this->projection = projection;
	this->setMVP();
}

void ModelLoader::translate(vec3 &trans){
	model = glm::translate(model, trans);
	this->setMVP();

}
void ModelLoader::rotateAroundSelf(float &angle, vec3 &axis){
	mat4 transform = mat4(1.0f);
	mat4 oldTransform = model;

	//model *= inverseModel;
	transform *= rotate(transform, angle, axis);

	model = oldTransform * transform;
	this->setMVP();

}
void ModelLoader::rotateAroundAxis(float &angle, vec3 &axis){
	model = rotate(model, angle, axis);
	this->setMVP();
}

void ModelLoader::scale(vec3 &scale){
	model = glm::scale(model, scale);

}

void ModelLoader::backToCenter(){
	mat4 inverseModel = inverse(model);
	model *= inverseModel;
	this->setMVP();
}

void ModelLoader::setMVP(){
	MV = view * model;
	MV3 = mat3(MV);
	MVP = projection * MV;
}

void ModelLoader::calculateTangents(){
	for (unsigned int i = 0; i < vertices.size(); i+= 3)
	{
		vec3 v1 = vertices.at(i);
		vec3 v2 = vertices.at(i+1);
		vec3 v3 = vertices.at(i+2);

		vec2 uv1 = uvs.at(i);
		vec2 uv2 = uvs.at(i+1);
		vec2 uv3 = uvs.at(i+2);


		vec3 deltaPos1 = vec3(v2.x - v1.x, v2.y - v1.y, v2.z - v1.z);
		vec3 deltaPos2 = vec3(v3.x - v1.x, v3.y - v1.y, v3.z - v1.z);

		vec2 deltaUV1 = vec2(uv2.x - uv1.x, uv2.y - uv1.y);
		vec2 deltaUV2 = vec2(uv3.x - uv1.x, uv3.y - uv1.y);


		float r = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);//(determinant(mat2(deltaUV1, deltaUV2)));

		vec3 tangent = (deltaPos1 * deltaUV2.y - deltaPos2 * deltaUV1.y) * r;
		vec3 bitangent = (deltaPos2 * deltaUV1.x - deltaPos1 * deltaUV2.x) * r;

		tangents.push_back(tangent);
		tangents.push_back(tangent);
		tangents.push_back(tangent);

		bitangents.push_back(bitangent);
		bitangents.push_back(bitangent);
		bitangents.push_back(bitangent);

	}
	for (unsigned int i = 0; i < vertices.size(); i++)
	{
		vec3 &tangent = tangents.at(i);
		vec3 &bitangent = bitangents.at(i);
		vec3 &normal = normals.at(i);
		// Gram Schmidt orthogonalisieren
		tangent = normalize((tangent - normal * dot(normal, tangent)));
		//Haendigkeit pruefen

		if(dot(cross(normal, tangent), bitangent) < 0.0f){
			tangent *= -1.0f;
		}
	}
}
