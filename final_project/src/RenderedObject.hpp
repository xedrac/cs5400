#ifndef RENDEREDOBJECT_HPP
#define RENDEREDOBJECT_HPP

#include <memory>
#include <vector>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_inverse.hpp"
#include "Mesh.hpp"
#include "Material.hpp"
#include "Texture.hpp"
#include "BoundingBox.hpp"

class RenderedObject;


typedef std::shared_ptr<RenderedObject> object_t;
// objectid_t is a unique identifier for any given object in the scene
typedef size_t objectid_t;
// this special objectid represents an uninitialized objectid
const objectid_t INVALID_OBJECTID = std::numeric_limits<objectid_t>::max();





// This is the base class for any object that is to
// be rendered in the scene.  It knows how to render itself
class RenderedObject
{
public:
    RenderedObject(GLuint program,
                   std::shared_ptr<Mesh> mesh,
                   glm::vec3 position,
                   glm::vec3 scale,
                   glm::vec3 rotation);

    objectid_t getId()                  const { return _id;          }
    std::shared_ptr<Mesh> getMesh()     const { return _mesh;        }
    const BoundingBox &getBoundingBox() const { return _boundingbox; }
    glm::mat4 getModelMatrix()          const { return _modelmatrix; }
    glm::mat3 getInvTranspModelMatrix() const { return _modelinvtranspmatrix; }
    glm::vec3 getPosition()             const { return _position;    }
    glm::vec3 getRotation()             const { return _rotation;    }
    glm::vec3 getScale()                const { return _scale;       }
    bool      getVisible()              const { return _visible;     }
    Material  getMaterial()             const { return _material;    }
    Texture   getTexture()              const { return _texture;     }

    // rotate/translate/scale the object
    void rotate(glm::vec3 axis, float theta);
    void translate(glm::vec3);
    void scale(glm::vec3);

    void setId(objectid_t id)             { _id = id;            }
    void setModelMatrix(glm::mat4 matrix) { _modelmatrix = matrix;
                                            _modelinvtranspmatrix = glm::inverseTranspose(glm::mat3(matrix)); }
    void setMaterial(const Material &m)   { _material = m;       }
    void setTexture (const Texture &t)    { _texture  = t;       }
    void setVisible (bool visible)        { _visible  = visible; }

    // Render the object
    void render(GLuint modelmatrixid, GLuint modelinvtranspmatrixid);

	// Update the object state, returns false if direction needs to change (TODO: need to fix this)
	virtual bool update(int elapsedms) = 0;

    // set a model matrix, or calculate it from _position, _scale, _rotation
	void calculateModelMatrix();

    // Determine if this object's boundingbox intersects with the boundingbox of 'other'
    bool intersects(const object_t &other);


protected:
    void storePoints();
    void storeNormals();
    void storeMesh();
    void storeTextureCoords();

protected:
    objectid_t _id;  // unique identifier given by the scene
    std::shared_ptr<Mesh> _mesh;
	BoundingBox _boundingbox;

private:
    glm::mat4 _modelmatrix;
    glm::mat3 _modelinvtranspmatrix;
	glm::vec3 _position;
	glm::vec3 _rotation;
    glm::vec3 _scale;
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

