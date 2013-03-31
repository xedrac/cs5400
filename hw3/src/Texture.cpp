
#include "Texture.hpp"
#include "stb_image/stb_image.h"
#include <iostream>

using namespace std;


Texture::Texture(const string &filename)
{
    int nchannels = 0;
    _data = stbi_load(filename.c_str(), (int *)&_width, (int *)&_height, &nchannels, 0);
    if (!_data) {
        cerr << "Error loading texture file " << filename << endl << endl;
    }

    glGenTextures(1, &_textureid);
    bindTexture();
}


Texture::~Texture()
{
   glDeleteTextures(1, &_textureid); 
}


void Texture::bindTexture()
{
    glBindTexture(GL_TEXTURE_2D, _textureid);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D,   //target
                 0,               //level, 0=base (no minimap)
                 GL_RGB,          //internal format
                 _width,          //image width
                 _height,         //image height
                 0,               //border
                 GL_RGB,          //format
                 GL_UNSIGNED_BYTE,//type
                 _data);          //image data
}

