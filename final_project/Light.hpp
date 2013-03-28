#ifndef LIGHT_HPP
#define LIGHT_HPP

#include "glm/glm.hpp"

class Light {
public:
    Light(const glm::vec3 &position,
          const glm::vec3 &diffuse,
          const glm::vec3 &specular)
        : _position(position),
          _diffuse(diffuse),
          _specular(specular),
          _constantattenuation(0.0),
          _linearattenuation(1.0),
          _quadraticattenuation(0.0),
          _spotcutoff(180.0),
          _spotexponent(0.0),
          _spotdirection(glm::vec3(0.0, 0.0, 0.0)),
          _emitting(true)
    {
    }

    Light() { }

    // Set the light position and emitting color(s)
    void setPosition(const glm::vec3 &position) { _position = position; }
    void setDiffuse (const glm::vec3 &color)    { _diffuse  = color;    }
    void setSpecular(const glm::vec3 &color)    { _specular = color;    }

    // Set the attenuation on the light
    void setConstantAttenuation(GLfloat value)  { _constantattenuation  = value; }
    void setLinearAttenuation(GLfloat value)    { _linearattenuation    = value; }
    void setQuadraticAttenuation(GLfloat value) { _quadraticattenuation = value; }

    // Vary spotlight effects 
    void setSpotCutoff(GLfloat value)           { _spotcutoff   = value; }
    void setSpotExponent(GLfloat value)         { _spotexponent = value; }
    void setSpotDirection(const glm::vec3 &dir) { _spotdirection = dir;  }

    // Turn the light on or off
    void setEmitting(bool emitting)             { _emitting = true; }

    // Accessors
    glm::vec3 getPosition()             const { return _position; }
    glm::vec3 getDiffuse()              const { return _diffuse;  }
    glm::vec3 getSpecular()             const { return _specular; }

    GLfloat   getConstantAttenuation()  const { return _constantattenuation;  }
    GLfloat   getLinearAttenuation()    const { return _linearattenuation;    }
    GLfloat   getQuadraticAttenuation() const { return _quadraticattenuation; }

    GLfloat   getSpotCutoff()           const { return _spotcutoff;    }
    GLfloat   getSpotExponent()         const { return _spotexponent;  }
    glm::vec3 getSpotDirection()        const { return _spotdirection; }
     
    bool      getEmitting()             const { return _emitting; }

private:
    glm::vec3 _position;
    glm::vec3 _diffuse;
    glm::vec3 _specular;
    GLfloat   _constantattenuation;
    GLfloat   _linearattenuation;
    GLfloat   _quadraticattenuation;
    GLfloat   _spotcutoff;
    GLfloat   _spotexponent;
    glm::vec3 _spotdirection;

    bool _emitting; 
};


#endif

