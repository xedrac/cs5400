
#include "Texture.hpp"
#include "stb_image/stb_image.h"
#include <iostream>

using namespace std;

#define GL_CLAMP_TO_EDGE 0x812F // for Microsoft header

Texture::Texture(const string &filename)
{
    _data = stbi_load(filename.c_str(), &_width, &_height, &_nchannels, 0);
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
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    GLenum format = GL_RGBA;
    if (_nchannels == 3) {
        format = GL_RGB;
    }
    
    glTexImage2D(GL_TEXTURE_2D,   //target
                 0,               //level, 0=base (no minimap)
                 format,          //internal format
                 _width,          //image width
                 _height,         //image height
                 0,               //border
                 format,          //format
                 GL_UNSIGNED_BYTE,//type
                 _data);
}

