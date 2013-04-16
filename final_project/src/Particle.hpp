#ifndef PARTICLE_HPP
#define PARTICLE_HPP

#include "glm/glm.hpp"

class Particle
{
public:
    Particle();
    Particle(glm::vec4 color, glm::vec3 position, glm::vec3 velocity);

    glm::vec4 getColor();
    glm::vec3 getPosition();
    glm::vec3 getVelocity();
    void setPosition(glm::vec3 position);
    void setVelocity(glm::vec3 velocity);
    void fadeout(float percent);

private:
    glm::vec4 _color;
    glm::vec3 _position;
    glm::vec3 _velocity;
    bool _visible;

    void hide();
};

#endif
