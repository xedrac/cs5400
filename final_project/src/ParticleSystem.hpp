#ifndef PARTICLESYSTEM_HPP
#define PARTICLESYSTEM_HPP

#include "Particle.hpp"
#include "glm/glm.hpp"
#include <vector>
#include <GL/glew.h>
#include <GL/freeglut.h>

class ParticleSystem
{
public:
    ParticleSystem(GLuint program, int size, GLfloat particleSize, glm::vec3 startposition);
    void render(GLuint modelmatrixid, GLuint viewmatrixid, GLuint projectionmatrixid, const glm::mat4 &viewmatrix, const glm::mat4 &projectionmatrix);
    bool update(int elapsedms); // return false if system has finished runtime

    void hide();

private:
    std::vector<Particle> _particles;
    bool _visible;
    GLuint _particleprogram;
    GLuint _glvertexbuffer;
    GLuint _glcolorbuffer;
    int _size;
    GLfloat _particlesize;
    int _totalelapsed;
    float _runtime;

    GLint _glvertexattrib;
    GLint _glcolorattrib;
};

#endif
