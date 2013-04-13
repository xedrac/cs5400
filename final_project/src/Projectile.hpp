#ifndef PROJECTILE_HPP
#define PROJECTILE_HPP

#include "glm/glm.hpp"
#include "RenderedObject.hpp"
#include "Mesh.hpp"


class Projectile : public RenderedObject
{
public:
    Projectile(glm::vec3 direction, float speed);

private:
	glm::vec3 _position;
	glm::vec3 _rotation;
	float     _speed;
	int       _direction; // probably just 0 or 1 but room for expansion
};



#endif

