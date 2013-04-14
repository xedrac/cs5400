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
    ParticleSystem(int size);
    void render();

private:
    std::vector<Particle> _particles;
    bool _visible;
    GLuint _glparticle;
    int _size;

    void hide();
};

#endif
