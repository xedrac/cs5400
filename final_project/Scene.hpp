#ifndef SCENE_HPP
#define SCENE_HPP

#include <GL/glew.h>
#include <GL/glut.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "Program.hpp"
#include "Light.hpp"
#include "RenderedObject.hpp"
#include <vector>


class Scene {
public:
    void init();

    // Load an object into the scene from a PLY file
    void loadObjectFromPly(const std::string &filename,
                           glm::mat4 modelmatrix = glm::translate(glm::mat4(), glm::vec3(0.0, -0.1, 0.0)));

    // Load the scene with PLY objects
    void clear();

    // Add a light to the scene
    void addLight(const Light &);

    // Sets the color/brightness of the ambient light
    // (0,0,0) is total darkness, (1.0,1,0,1,0) is total light
    //void setAmbientLight(glm::vec3 rgb);


    // render all of the objects in the scene
    void render(const glm::vec3 &eyeposition,
                const glm::vec3 &lookdirection,
                const glm::vec3 &updirection,
                const glm::mat4 &projectionmatrix);

    std::shared_ptr<cs5400::Program> getProgram() { return _program; }

private:
    std::shared_ptr<cs5400::Program> _program;
    
    // All objects to be rendered in the scene
    std::vector<RenderedObject> _objects;

    std::vector<Light> _lights;
    //glm::vec3 _ambientlight;

    GLuint _vertex;
    GLuint _vertexnormal;
    GLuint _glModelMatrix;
    GLuint _glViewMatrix;
    GLuint _glProjectionMatrix;

    GLuint _glLightPosition;
    GLuint _glLightAmbient;
    GLuint _glLightDiffuse;
    GLuint _glLightSpecular;
    GLuint _glLightCount;
};


#endif
