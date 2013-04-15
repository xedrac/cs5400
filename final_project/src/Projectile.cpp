#include "glm/glm.hpp"
#include "Projectile.hpp"

using namespace std;


Projectile::Projectile(GLuint program,
                       shared_ptr<Mesh> mesh,
                       glm::vec3 position,
                       glm::vec3 scale,
                       glm::vec3 rotation,
                       glm::vec3 direction,
                       float speed)
    : RenderedObject(program, mesh, position, scale, rotation),
      _direction(direction),
      _speed(speed)
{

}


// Returns true if projectile has moved beyond the screen
bool Projectile::update(int elapsedms)
{
    _boundingbox.update(getModelMatrix());
    translate(_direction * (_speed*elapsedms));

    return (getPosition().y > 1.0 || getPosition().y < -1.0);
}

