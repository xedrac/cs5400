#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>
#include <vector>
#include <GL/freeglut.h>


class Texture
{
public:
    Texture(const std::string &filename);     
    ~Texture();

    unsigned int width()                      const { return _width;  }
    unsigned int height()                     const { return _height; }

    void bindTexture();
    GLuint textureId()                        const { return _textureid; }

private:
    bool loadUsingSDLImage(const std::string &filename);

private:
    GLuint _textureid;
    int _width;
    int _height;
    int _nchannels;
    unsigned char *_data;
};

#endif
