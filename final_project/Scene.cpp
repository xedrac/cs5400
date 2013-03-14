// Allows converting from glm::vec4 to glm::vec3 easily
#include <GL/glew.h>
#include <GL/glut.h>
#include "PlyParser.hpp"
#include "Scene.hpp"
#include "Mountain.hpp"
#include "Program.hpp"
#include <assert.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

using namespace std;


void Scene::init()
{
    _program = cs5400::make_program(cs5400::make_vertexShader("shaders/vertex.glsl"),
                                    cs5400::make_fragmentShader("shaders/fragment.glsl"));
    GLuint handle = _program->getHandle();

    _vertex       = glGetAttribLocation(handle, "vertex");
    _vertexnormal = glGetAttribLocation(handle, "vertexnormal");

    _glModelMatrix      = glGetUniformLocation(handle, "M");
    _glViewMatrix       = glGetUniformLocation(handle, "V");
    _glProjectionMatrix = glGetUniformLocation(handle, "P");

    _glLightPosition    = glGetUniformLocation(handle, "lightpos_world");
    _glLightAmbient     = glGetUniformLocation(handle, "lightambient");
    _glLightDiffuse     = glGetUniformLocation(handle, "lightdiffuse");
    _glLightSpecular    = glGetUniformLocation(handle, "lightspecular");
    _glLightCount       = glGetUniformLocation(handle, "lightcount");
}


void Scene::loadObjectFromPly(const string &filename, glm::mat4 modelmatrix)
{
    shared_ptr<Mesh> mesh = parsePlyFile(filename); 
    RenderedObject obj(_program->getHandle(), mesh);
    obj.setModelMatrix(modelmatrix);
    _objects.push_back(obj);
}


void Scene::addLight(const Light &light)
{
    _lights.push_back(light);
}


// Render the scene as it currently is
void Scene::render(const glm::vec3 &eyeposition,
                   const glm::vec3 &lookdirection,
                   const glm::vec3 &updirection,
                   const glm::mat4 &projectionmatrix)
{
    glUseProgram(_program->getHandle());

    glm::mat4 viewmatrix = glm::lookAt(eyeposition, lookdirection, updirection);

    // pass our transformation matricies to the shaders
    glUniformMatrix4fv(_glViewMatrix, 1, GL_FALSE, glm::value_ptr(viewmatrix));
    glUniformMatrix4fv(_glProjectionMatrix, 1, GL_FALSE, glm::value_ptr(projectionmatrix));

    // don't allow more than 10 lights for now...
    if (_lights.size() > 10)
        _lights.resize(10);

    // tell the shaders about lighting inforamtion
    vector<glm::vec4> lightpos;
    vector<glm::vec4> lightambient;
    vector<glm::vec4> lightdiffuse;
    vector<glm::vec4> lightspecular;
    for (size_t i=0; i<_lights.size(); i++) {
        const Light &light = _lights[i];
        //glm::vec3 ambient = light.getAmbient();
        //glm::vec3 diffuse = light.getDiffuse();
        //glm::vec3 specular = light.getSpecular();

        lightpos.push_back(glm::vec4(light.getPosition(), 0.0));
        lightambient.push_back(glm::vec4(light.getAmbient(), 1.0));
        lightdiffuse.push_back(glm::vec4(light.getDiffuse(), 1.0));
        lightspecular.push_back(glm::vec4(light.getSpecular(), 1.0));
    }

    glUniform4fv(_glLightPosition, 10, glm::value_ptr(lightpos[0]));
    glUniform4fv(_glLightAmbient,  10, glm::value_ptr(lightambient[0]));
    glUniform4fv(_glLightDiffuse,  10, glm::value_ptr(lightdiffuse[0]));
    glUniform4fv(_glLightSpecular, 10, glm::value_ptr(lightspecular[0]));
    glUniform1i(_glLightCount, (GLint)_lights.size());

    // now render each object
    for (size_t i=0; i<_objects.size(); i++) {
        _objects[i].render(_glModelMatrix);
    }
}

