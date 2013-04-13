#include <vector>
#include <GL/glew.h>
#include "Spaceship.hpp"
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
	  _direction(0)  // might want to be able to pass this in, but for now we'll default to one direction
{
    Material m;
    m.setAmbient(glm::vec3(0.2, 0.2, 0.2));
    m.setDiffuse(glm::vec3(0.8, 1.0, 0.8));
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

	if (_direction == 0) {
		//this->rotate(glm::vec3(0.0, 1.0, 0.0), this->_speed * 500 * elapsedms);
		translate(glm::vec3(_speed * elapsedms, 0.0, 0.0));
		//return !(this->_position.x > 0.11);
		return !(position.x > 0.15); // make it bigger to show off collision
	} else {
		//this->rotate(glm::vec3(0.0, 1.0, 0.0), this->_speed * -500 * elapsedms);
		translate(glm::vec3(-_speed * elapsedms, 0.0, 0.0));
		//return !(this->_position.x < -0.09);
		return !(position.x < -0.13); // make it bigger to show off collision
	}
}

// Change the object's direction
void Spaceship::changeDirection()
{
	// simple 0 <-> 1 for now
	_direction = _direction ^ 1;
}


#if 0
// Naive implementation
bool Spaceship::intersects(std::vector<Spaceship>* set)
{
	for(size_t i = 0; i < set->size(); i++) {
		if (bounds.intersects(&set->at(i).bounds))
			return true;
	}
	return false;
}

glm::mat4 Spaceship::getTransformationMatrix(bool includeRotation)
{
	glm::mat4 modelTranslate = glm::translate(glm::mat4(1.0), _position);
	if (includeRotation)
	{
		glm::mat4 modelRotateX = glm::rotate(modelTranslate, _rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
		glm::mat4 modelRotateY = glm::rotate(modelRotateX, _rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
		glm::mat4 model = glm::rotate(modelRotateY, _rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
		return model;
	}
	else
	{
		return modelTranslate;
	}
}
#endif
