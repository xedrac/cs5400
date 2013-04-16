#ifndef PARTICLESYSTEM_HPP
#define PARTICLESYSTEM_HPP

#include "Particle.hpp"
#include "glm/glm.hpp"
#include <vector>
#include <GL/glew.h>
#include <GL/freeglut.h>

enum ParticleSystemType
{
	Explosion = 0,
	EnemyExhaust = 1,
	PlayerExhaust = 2
};

class ParticleSystem
{
public:
    ParticleSystem(GLuint program, ParticleSystemType type, glm::vec3 startposition);
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
    ParticleSystemType _type;

    GLint _glvertexattrib;
    GLint _glcolorattrib;

    void createExplosion(glm::vec3 position);
    void createPlayerExhaust(glm::vec3 position);
};

#endif
