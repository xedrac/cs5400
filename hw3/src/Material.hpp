#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include "glm/glm.hpp"

class Material {
public:
    Material()
        : _ambient (glm::vec3(0.2, 0.2, 0.2)),
          _diffuse (glm::vec3(1.0, 0.8, 0.8)),
          _specular(glm::vec3(1.0, 1.0, 1.0)),
          _shininess(5.0)
    {
    }

    void setAmbient (const glm::vec3 &color) { _ambient   = color; }
    void setDiffuse (const glm::vec3 &color) { _diffuse   = color; }
    void setSpecular(const glm::vec3 &color) { _specular  = color; }
    void setShininess(GLfloat value)         { _shininess = value; }

    glm::vec3 getAmbient() const   { return _ambient;  }
    glm::vec3 getDiffuse() const   { return _diffuse;  }
    glm::vec3 getSpecular() const  { return _specular; }
    GLfloat   getShininess() const { return _shininess;}

private:
    glm::vec3 _ambient;
    glm::vec3 _diffuse;
    glm::vec3 _specular;
    GLfloat   _shininess;
};


#endif

