#ifndef SPACESHIP_HPP
#define SPACESHIP_HPP

#include "glm/glm.hpp"
#include "RenderedObject.hpp"
#include "Mesh.hpp"


class Spaceship : public RenderedObject
{
public:
    Spaceship(GLuint program,
              std::shared_ptr<Mesh> mesh,
              const glm::vec3 &position,
              const glm::vec3 &scale,
              const glm::vec3 &rotation,
              int direction,
              float speed);

	// notifies the object to change direction
    // TODO: Make this more generic, like setDirection(glm::vec3)
	void changeDirection();
    bool update(int elapsedms);

private:
	float     _speed;
	int       _direction; // probably just 0 or 1 but room for expansion
};


#endif

