#ifndef SCENE_HPP
#define SCENE_HPP

#include <GL/glew.h>
#include <GL/freeglut.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "Program.hpp"
#include "Light.hpp"
#include "RenderedObject.hpp"
#include "ParticleSystem.hpp"
#include <vector>




class Scene
{
public:
    void init();

    // Load an object into the scene from a PLY file
    void loadObjectFromPly(const std::string &filename, int direction, 
                           glm::mat4 modelmatrix = glm::translate(glm::mat4(1.0), glm::vec3(0.0, -0.1, 0.0)));

    // Load the scene with PLY objects
    void clear();

    // Add a light to the scene
    void addLight(const Light &);

    // Sets the color/brightness of the ambient light
    // (0,0,0) is total darkness, (1.0,1,0,1,0) is total light
    void setAmbientLight(const glm::vec3 &rgb) { _ambientlight = rgb; }


    // Returns a new id to represent an object in the scene
    objectid_t getNewObjectId();
    void insertObject(object_t obj);
    void removeObject(object_t obj);

    void insertParticleSystem(std::shared_ptr<ParticleSystem> ps);

    // render all of the objects in the scene
    void render(const glm::vec3 &eyeposition,
                const glm::vec3 &lookdirection,
                const glm::vec3 &updirection,
                const glm::mat4 &projectionmatrix);


    std::shared_ptr<cs5400::Program> getProgram() { return _program; }

private:
    std::shared_ptr<cs5400::Program> _program;
    
    // All objects to be rendered in the scene
    std::vector<object_t> _objects;
    std::vector< std::shared_ptr<ParticleSystem> > _particlesystems;

    std::vector<Light> _lights;
    glm::vec3 _ambientlight;

    GLuint _glModelMatrix;
    GLuint _glViewMatrix;
    GLuint _glProjectionMatrix;
    GLuint _glModelInvTranspMatrix;
    GLuint _glViewInverseMatrix;

    GLuint _glLightPosition;
    GLuint _glLightAmbient;
    GLuint _glLightDiffuse;
    GLuint _glLightSpecular;
    GLuint _glLightAttenuation;
    GLuint _glLightSpotCutoff;
    GLuint _glLightSpotExponent;
    GLuint _glLightSpotDirection;
    GLuint _glLightCount;
    GLuint _glSceneAmbient;

    GLuint _glCameraPosition;
};


#endif
