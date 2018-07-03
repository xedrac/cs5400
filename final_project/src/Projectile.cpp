#include "glm/glm.hpp"
#include "Projectile.hpp"
#include "RNG.hpp"

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
    Material m;
    m.setAmbient(glm::vec3(0.1, 0.1, 0.1));
    //m.setDiffuse(glm::vec3(0.831, 0.628, 0.215));
    m.setDiffuse(glm::vec3(1.0, 0.5, 0.0));
    m.setSpecular(glm::vec3(1.0, 1.0, 1.0));
    m.setShininess(100.0);
    setMaterial(m);
}


// Returns true if projectile has moved beyond the screen
bool Projectile::update(int elapsedms)
{
    _boundingbox.update(getModelMatrix());
    translate(_direction * (_speed*elapsedms));

    rotate(glm::vec3(0.0,1.0,0.0), 10.0);
    //rotate(glm::vec3(RNG::genNormalizedFloat(),RNG::genNormalizedFloat(),RNG::genNormalizedFloat()), 5.0);

    return (getPosition().y > 1.0 || getPosition().y < -1.0);
}

