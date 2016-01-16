#include "Texture.h"
#include <iostream>

Texture::Texture()
    : texture(0)
    , textureLocation(0)
    , location(0)
    , width(0)
    , height(0)
    , data(0)
{
    glGenTextures(1, &texture);
}

Texture::Texture(const char* imagePath)
    : texture(0)
    , textureLocation(0)
    , location(0)
    , width(0)
    , height(0)
    , data(0)
{
    texture = SOIL_load_OGL_texture(imagePath, SOIL_LOAD_RGBA, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_COMPRESS_TO_DXT);
    if (texture == NULL){
        std::cout << imagePath << " Problem! Fehler: " << SOIL_last_result()  << std::endl;
    }
}

Texture::~Texture()
{
    glDeleteTextures(1, &texture);
}

void Texture::setData(unsigned char& data)
{
    this->data = data;
}

void Texture::loadCommonOptions()
{
    this->bind();

    glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );

    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );

    /* gluBuild2DMipmaps( GL_TEXTURE_2D, 3, this->width, this->height, GL_RGB, GL_UNSIGNED_BYTE, this->data ); */
}

void Texture::bind()
{
    glActiveTexture(GL_TEXTURE0 + this->location);
    glBindTexture(GL_TEXTURE_2D, this->texture);
    glUniform1i(textureLocation, this->location);
}

void Texture::unbind()
{
    glBindTexture(GL_TEXTURE_2D, 0);
}

GLuint Texture::getTextureLoc()
{
    return textureLocation;
}

GLuint Texture::getTexture()
{
    return texture;
}

void Texture::setTexture(GLint shaderProgram, GLint location, const char* texture_name){
	textureLocation = glGetUniformLocation(shaderProgram, texture_name);
	this->location = location;

	// when texture area is small, bilinear filter the closest mipmap
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	// when texture area is large, bilinear filter the first mipmap
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);


	// texture should tile
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// build our texture mipmaps
	//gluBuild2DMipmaps(GL_TEXTURE_2D, 4, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data);

}

