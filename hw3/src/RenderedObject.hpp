#ifndef RENDEREDOBJECT_HPP
#define RENDEREDOBJECT_HPP

#include <memory>
#include <vector>
#include "glm/glm.hpp"
#include "Mesh.hpp"
#include "Material.hpp"
#include "Texture.hpp"

// This is the base class for any object that is to
// be rendered in the scene.  It knows how to render
// itself


class RenderedObject {
public:
    RenderedObject(GLuint program, std::shared_ptr<Mesh> mesh);

    // Rotate the object along the arbitrary axis, by 'theta' degrees
    void rotate(glm::vec3 axis, double theta);

    // Move the object along the x, y, and z axes in the amount specified
    void translate(glm::vec3 xyz);

    // Objects that are not 'visible' will not be rendered
    void setVisible(bool visible);

    // Set the matrix to convert from model coords to world coords
    void setModelMatrix(glm::mat4 matrix);

    // Set the material of the object
    void setMaterial(const Material &m);

    // Render the object
    void render(GLuint modelmatrixid, GLuint modelinvtranspmatrixid);

    // Access the mesh data (probably should enforce const or something)
    std::shared_ptr<Mesh> getMesh() { return _mesh; }

private:
    void storePoints();
    void storeNormals();
    void storeMesh();
    void storeTextureCoords();

private:
    std::shared_ptr<Mesh> _mesh;
    glm::mat4 _modelmatrix;
    glm::mat3 _modelinvtranspmatrix;
    bool _visible;

    GLuint _glvertexbuffer;
    GLuint _glnormalbuffer;
    GLuint _glmeshbuffer;
    GLuint _gluvbuffer;
    GLuint _gltexturesampler;

    GLint _glvertexattrib;
    GLint _glnormalattrib;
    GLint _gluvattrib;

    GLuint _glshininess;
    GLuint _glambient;
    GLuint _gldiffuse;
    GLuint _glspecular;

    Material _material;
    Texture _texture;
};



#endif

