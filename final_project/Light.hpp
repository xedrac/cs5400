#ifndef LIGHT_HPP
#define LIGHT_HPP

#include "glm/glm.hpp"

class Light {
public:
    Light(const glm::vec3 &position,
          const glm::vec3 &ambient,
          const glm::vec3 &diffuse,
          const glm::vec3 &specular)
        : _position(position),
          _ambient(ambient),
          _diffuse(diffuse),
          _specular(specular),
          _emitting(true)
    {
    }

    Light() { }

    // Set the light position, color and emitting power
    void setPosition(glm::vec3 position) { _position = position; }
    void setAmbient(glm::vec3 color)     { _ambient = color; }
    void setDiffuse(glm::vec3 color)     { _diffuse = color; }
    void setSpecular(glm::vec3 color)    { _specular= color; }

    // Turn the light on or off
    void setEmitting(bool emitting)      { _emitting = true; }

    // Accessors
    glm::vec3 getPosition() const { return _position; }
    glm::vec3 getAmbient()  const { return _ambient;  }
    glm::vec3 getDiffuse()  const { return _diffuse;  }
    glm::vec3 getSpecular() const { return _specular; }
    bool      getEmitting() const { return _emitting; }

private:
    glm::vec3 _position;
    glm::vec3 _ambient;
    glm::vec3 _diffuse;
    glm::vec3 _specular;

    bool _emitting; 
};


#endif

