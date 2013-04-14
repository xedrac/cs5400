#include "ParticleSystem.hpp"

ParticleSystem::ParticleSystem(int size) : _size(size)
{
    // let's make some particles
    for (int i = 0; i < _size; i++)
    {
        Particle p(glm::vec4(1.0, 0.0, 0.0, 1.0),                 // color
                   glm::vec4(0.0),                                // position
                   glm::vec3(0.0001 * ((float)rand()/RAND_MAX),   // velocity
                             0.0001 * ((float)rand()/RAND_MAX),
                             0.0001 * ((float)rand()/RAND_MAX)),
                   1.0);                                          // mass

        _particles.push_back(p);
    }

    glGenBuffers(1, &_glparticle);
}

// Render the system
void ParticleSystem::render()
{
    // Don't render if it's been flagged as not visible
    if (!_visible)
        return;

    std::vector<glm::vec4> points;
    std::vector<glm::vec4> colors;

    for (size_t i = 0; i < _size; i++)
    {
        points.push_back(_particles[i].getPosition());
        colors.push_back(_particles[i].getColor());
    }

    glBindBuffer(GL_ARRAY_BUFFER, _glparticle);
    glBufferData(GL_ARRAY_BUFFER, _size * sizeof(glm::vec4) * 2, NULL, GL_DYNAMIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, _size * sizeof(glm::vec4), &points[0]);
    glBufferSubData(GL_ARRAY_BUFFER, _size * sizeof(glm::vec4), _size * sizeof(glm::vec4), &colors[0]);
    glDrawArrays(GL_POINTS, 0, _size);
    glutSwapBuffers();
}