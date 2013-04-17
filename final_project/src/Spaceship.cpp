#include <vector>
#include <GL/glew.h>
#include "Spaceship.hpp"
#include "RNG.hpp"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/matrix_inverse.hpp"
#include "glm/gtc/type_ptr.hpp"

using namespace std;


Spaceship::Spaceship(GLuint program,
                     shared_ptr<Mesh> mesh,
                     const glm::vec3 &position,
                     const glm::vec3 &scale,
                     const glm::vec3 &rotation,
                     int direction,
                     float speed)
    : RenderedObject(program, mesh, position, scale, rotation),
	  _speed(speed),
	  _direction(direction),
      _timesincelastprojectile(-1),
      _timetonextprojectile(0)
{
    Material m;
    m.setAmbient(glm::vec3(0.2, 0.3, 0.2));
    m.setDiffuse(glm::vec3(0.7, 1.0, 0.7));
    m.setSpecular(glm::vec3(1.0, 1.0, 1.0));
    m.setShininess(100.0);
    setMaterial(m);
}


// Update the object's state. Returns false if direction needs to change
// TODO:  We need a way to enforce dynamic bounds instead of hard-coded bounds...
bool Spaceship::update(int elapsedms)
{
	// first update this object's bounding box
    _boundingbox.update(getModelMatrix());
    glm::vec3 position = getPosition();

    bool keepdirection = true;

    switch (_direction) {
        case 0:
            translate(glm::vec3( _speed * elapsedms, 0.0, 0.0));
            keepdirection = !(position.x > 1.0);
            break;
        case 1:
            translate(glm::vec3(-_speed * elapsedms, 0.0, 0.0));
            keepdirection = !(position.x < -1.0);
            break;
        default:
            break;
    }

    _timesincelastprojectile += elapsedms;
    return keepdirection;
}

// Change the object's direction
void Spaceship::changeDirection()
{
	// simple 0 <-> 1 for now
	_direction = _direction ^ 1;
}

