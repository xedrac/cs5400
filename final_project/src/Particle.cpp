#include "Particle.hpp"

Particle::Particle() 
{ 
    _color = glm::vec4(0.0);
    _position = glm::vec3(0.0); 
    _velocity = glm::vec3(0.0); 
    _visible = true;
};

Particle::Particle(glm::vec4 color, glm::vec3 position, glm::vec3 velocity)
{
    _color = color;
    _position = position;
    _velocity = velocity;
    _visible = true;
};


glm::vec4 Particle::getColor()
{
    return _color;
}

glm::vec3 Particle::getPosition()
{
    return _position;
}

glm::vec3 Particle::getVelocity()
{
    return _velocity;
}

void Particle::setPosition(glm::vec3 position)
{
    _position = position;
}

void Particle::setVelocity(glm::vec3 velocity)
{
    _velocity = velocity;
}

void Particle::fadeout(float percent)
{
    _color.a -= percent;
}

void Particle::hide()
{
    _visible = false;
}