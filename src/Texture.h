#ifndef TEXTURE_H_
#define TEXTURE_H_

#include <GL/glew.h>
//#include <SOIL/SOIL.h>
#include<FreeImage.h>
#include <iostream>

/*
 * Texture class. Use in the following order:
 * 1.) Constructor
 * 2.) bind()
 * 3.) setData()
 * 4.) loadCommonOptions()
 * 5.) linkTexture
 */
using namespace std;
class Texture
{

    public:
        Texture();
        Texture(const char* imagePath );
        ~Texture();

        GLuint getTexture();
        GLuint getTextureLoc();

        void linkTexture(GLint shaderProgram, const char *texture_name);
        void setData(unsigned char* data, int width, int height);
        void setData(float* data, int width, int height, int depth = 1);
        void setResolution(int width, int height) { this->width = width; this->height = height; };
        void loadGPositionOptions();
        void loadGNormalOptions();
        void loadGAlbedoOptions();
        void loadCommonOptions();
        void loadSkydomeOptions();
        void loadSkydome3DOptions();
        void loadHeightmapOptions();
        void loadNormalMapOptions();
        void loadWaterOptions();
        void loadWaterNormalOptions();
        void bind();
        void bind3D();
        void unbind();
        void printCData();


    private:
        static int idGenerator;
        GLuint texture;
        GLuint textureLocation;
        GLint location;

        int width, height, depth;
        unsigned char* cdata;
        float* ddata;

};
#endif /* TEXTURE_H_ */
