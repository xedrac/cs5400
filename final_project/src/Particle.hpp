#ifndef PARTICLE_HPP
#define PARTICLE_HPP

#include "glm/glm.hpp"

class Particle
{
public:
    Particle();
    Particle(glm::vec4 color, glm::vec4 position, glm::vec3 velocity, float mass);

    glm::vec4 getColor();
    glm::vec4 getPosition();

private:
    glm::vec4 _color;
    glm::vec4 _position;
    glm::vec3 _velocity;
    float _mass;
    bool _visible;

    void hide();
};

#endif
