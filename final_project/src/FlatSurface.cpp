#include "glm/glm.hpp"
#include "FlatSurface.hpp"

using namespace std;


FlatSurface::FlatSurface(GLuint program,
                        shared_ptr<Mesh> mesh,
                        glm::vec3 position,
                        glm::vec3 scale,
                        glm::vec3 rotation)
    : RenderedObject(program, mesh, position, scale, rotation)
{
    Material m;
    m.setAmbient(glm::vec3(0.2, 0.2, 0.2));
    m.setDiffuse(glm::vec3(0.8, 0.8, 0.8));
    m.setSpecular(glm::vec3(0.0, 0.0, 0.0));
    m.setShininess(1.0);
    setMaterial(m);
}


bool FlatSurface::update(int elapsedms)
{
    return true;
}

