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
    virtual bool update(int elapsedms);

    float getSpeed() const      { return _speed;     }
    void  setSpeed(float speed) { _speed = speed;    }

    int   getDirection() const  { return _direction; }
    void  setDirection(int dir) { _direction = dir;  }

private:
	float _speed;
	int   _direction; // probably just 0 or 1 but room for expansion
};


#endif

