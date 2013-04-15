#ifndef PROJECTILE_HPP
#define PROJECTILE_HPP

#include <memory>
#include "glm/glm.hpp"
#include "RenderedObject.hpp"
#include "Mesh.hpp"


class Projectile : public RenderedObject
{
public:
    Projectile(GLuint program,
               std::shared_ptr<Mesh> mesh,
               glm::vec3 position,
               glm::vec3 scale,
               glm::vec3 rotation,
               glm::vec3 direction,
               float speed);
    virtual ~Projectile() {}

    virtual bool update(int elapsedms);

private:
	glm::vec3 _direction;
	float _speed;
};



#endif

