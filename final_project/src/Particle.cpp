#include "Particle.hpp"

Particle::Particle() 
{ 
    _color = _position = glm::vec4(0.0); 
    _velocity = glm::vec3(0.0); 
    _mass = 1.0; 
    _visible = true;
};

Particle::Particle(glm::vec4 color, glm::vec4 position, glm::vec3 velocity, float mass)
{
    _color = color;
    _position = position;
    _velocity = velocity;
    _mass = mass;
    _visible = true;
};


glm::vec4 Particle::getColor()
{
    return _color;
}

glm::vec4 Particle::getPosition()
{
    return _position;
}

void Particle::hide()
{
    _visible = false;
}