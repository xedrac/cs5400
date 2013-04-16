#ifndef FLATSURFACE_HPP
#define FLATSURFACE_HPP

#include <memory>
#include "glm/glm.hpp"
#include "RenderedObject.hpp"
#include "Mesh.hpp"


class FlatSurface : public RenderedObject
{
public:
    FlatSurface(GLuint program,
                std::shared_ptr<Mesh> mesh,
                glm::vec3 position,
                glm::vec3 scale,
                glm::vec3 rotation);

    ~FlatSurface() {}

    bool update(int elapsedms);
};



#endif

