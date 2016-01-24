#include "Texture.h"
int Texture::idGenerator = 0;

Texture::Texture()
    : texture(0)
    , textureLocation(0)
    , location(0)
    , width(0)
    , height(0)
	, depth(1)
    , cdata(0)
    , ddata(0)
{
    this->location = idGenerator++;
    glGenTextures(1, &texture);
}

Texture::Texture(const char* imagePath)
    : texture(0)
    , textureLocation(0)
    , location(0)
    , width(0)
    , height(0)
    , cdata(0)
    , ddata(0)
{
	FreeImage_Initialise();
	glGenTextures(1, &texture);
	this->location = idGenerator++;

	FREE_IMAGE_FORMAT fif = FreeImage_GetFileType(imagePath, 0);

	if (fif == FIF_UNKNOWN)
		{
			cout << "Unbekannter Typ" << endl;
			cout.flush();
		}

	FIBITMAP* pImage = FreeImage_Load(
	        fif,
			imagePath);
	if (!FreeImage_HasPixels(pImage))
		{
			cout << "No Pixels" << endl;
			cout.flush();
		}
	if (FreeImage_GetBPP(pImage) != 32)
	{
		cout << "Konvertiere zu 32 bits" << endl;
		cout.flush();
	    FIBITMAP* tempImage = pImage;
	    pImage = FreeImage_ConvertTo32Bits(tempImage);
	}
//	FIBITMAP* tempImage = pImage;
//	pImage = FreeImage_ConvertTo24Bits(tempImage);
//	pImage = FreeImage_ConvertTo24Bits(pImage);
	width = FreeImage_GetWidth(pImage);
	height = FreeImage_GetHeight(pImage);

	cdata = FreeImage_GetBits(pImage);

//		{
//			cdata[pix * 4 + 0] = pixels[pix * 4 + 2];
//			cdata[pix * 4 + 1] = pixels[pix * 4 + 1];
//			cdata[pix * 4 + 2] = pixels[pix * 4 + 0];
//			cdata[pix * 4 + 3] = pixels[pix * 4 + 3]; // Add this line to copy Alpha
//		}

//	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, nWidth, nHeight,
//	    0, GL_BGRA, GL_UNSIGNED_BYTE, (void*)FreeImage_GetBits(pImage));

//	FreeImage_Unload(pImage);

}

Texture::~Texture()
{
    glDeleteTextures(1, &texture);
    FreeImage_DeInitialise();
}

void Texture::setData(unsigned char* data, int width, int height)
{
    this->cdata     = data;
    this->width     = width;
    this->height    = height;
}

void Texture::setData(float* data, int width, int height, int depth)
{
	this->ddata     = data;
	this->cdata		= new unsigned char[width * height * depth];
//	for(int i = 0; i < width * height * depth; i++)
//	{
//		this->cdata[i] = (int)(data[i] * 255);
//	}
	this->width     = width;
	this->height    = height;
	this->depth		= depth;
}

void Texture::loadCommonOptions()
{
//	glActiveTexture(GL_TEXTURE0 + this->location);
//	glBindTexture(GL_TEXTURE_2D, this->texture);

	// Give the image to OpenGL
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, this->width, this->height, 0, GL_BGRA, GL_UNSIGNED_BYTE, cdata);

    // build our texture mipmaps
    //gluBuild2DMipmaps(GL_TEXTURE_2D, 4, width, height, GL_RGBA, GL_UNSIGNED_BYTE, cdata);

    // select modulate to mix texture with color for shading
    glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );

    // when texture area is small, bilinear filter the closest mipmap
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    // when texture area is large, bilinear filter the first mipmap
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // texture should tile
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,  GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,  GL_CLAMP_TO_EDGE);

}

void Texture::loadSkydomeOptions()
{
//	glActiveTexture(GL_TEXTURE0 + this->location);
//	glBindTexture(GL_TEXTURE_2D, this->texture);

	// Give the image to OpenGL
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, this->width, this->height, 0, GL_RED, GL_FLOAT, ddata);

    // build our texture mipmaps
    //gluBuild2DMipmaps(GL_TEXTURE_2D, 4, width, height, GL_RGBA, GL_UNSIGNED_BYTE, cdata);

    // select modulate to mix texture with color for shading
    glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );

    // when texture area is small, bilinear filter the closest mipmap
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    // when texture area is large, bilinear filter the first mipmap
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // texture should tile
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,  GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,  GL_REPEAT);

}

void Texture::loadSkydome3DOptions()
{
//	glActiveTexture(GL_TEXTURE0 + this->location);
//	glBindTexture(GL_TEXTURE_2D, this->texture);

	// Give the image to OpenGL
    glTexImage3D(GL_TEXTURE_3D, 0, GL_RED, this->width, this->height, this->depth, 0, GL_RED, GL_FLOAT, ddata);

    // build our texture mipmaps
    //gluBuild2DMipmaps(GL_TEXTURE_2D, 4, width, height, GL_RGBA, GL_UNSIGNED_BYTE, cdata);

    // select modulate to mix texture with color for shading
    glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );

    // when texture area is small, bilinear filter the closest mipmap
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    // when texture area is large, bilinear filter the first mipmap
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // texture should tile
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S,  GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T,  GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R,  GL_CLAMP_TO_EDGE);

}
void Texture::loadHeightmapOptions()
{

    // Give the image to OpenGL
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, this->width, this->height, 0, GL_RED, GL_FLOAT, ddata);


    // build our texture mipmaps
    /* gluBuild2DMipmaps(GL_TEXTURE_2D, 4, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data); */

    // select modulate to mix texture with color for shading
    glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );

    // when texture area is small, bilinear filter the closest mipmap
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    // when texture area is large, bilinear filter the first mipmap
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // texture should tile
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

}

void Texture::bind()
{
    glActiveTexture(GL_TEXTURE0 + this->location);
    glBindTexture(GL_TEXTURE_2D, this->texture);
}

void Texture::bind3D()
{
    glActiveTexture(GL_TEXTURE0 + this->location);
    glBindTexture(GL_TEXTURE_3D, this->texture);
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

void Texture::linkTexture(GLint shaderProgram, const char* texture_name)
{

    glUseProgram(shaderProgram);

    textureLocation = glGetUniformLocation(shaderProgram, texture_name);
    glUniform1i(textureLocation, location);
    cout << textureLocation;
    cout.flush();

    // free buffer
    /* free(data); */

}
