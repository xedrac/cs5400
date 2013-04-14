// Allows converting from glm::vec4 to glm::vec3 easily
#include <GL/glew.h>
#include <GL/freeglut.h>
#include "PlyParser.hpp"
#include "Scene.hpp"
#include "Program.hpp"
#include <assert.h>
#include <iostream>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/matrix_inverse.hpp"
#include "glm/gtc/type_ptr.hpp"

using namespace std;


void Scene::init()
{
    _program = cs5400::make_program(cs5400::make_vertexShader("shaders/vertex.glsl"),
                                    cs5400::make_fragmentShader("shaders/fragment.glsl"));
    GLuint handle = _program->getHandle();

    _glModelMatrix          = glGetUniformLocation(handle, "M");
    _glViewMatrix           = glGetUniformLocation(handle, "V");
    _glProjectionMatrix     = glGetUniformLocation(handle, "P");
    _glModelInvTranspMatrix = glGetUniformLocation(handle, "M_inv_transpose");
    _glViewInverseMatrix    = glGetUniformLocation(handle, "V_inv");

    _glLightPosition      = glGetUniformLocation(handle, "lightpos_ws");
    _glLightAmbient       = glGetUniformLocation(handle, "lightambient");
    _glLightDiffuse       = glGetUniformLocation(handle, "lightdiffuse");
    _glLightSpecular      = glGetUniformLocation(handle, "lightspecular");
    _glLightAttenuation   = glGetUniformLocation(handle, "lightattenuation");
    _glLightSpotCutoff    = glGetUniformLocation(handle, "lightspotcutoff");
    _glLightSpotExponent  = glGetUniformLocation(handle, "lightspotexponent");
    _glLightSpotDirection = glGetUniformLocation(handle, "lightspotdirection");
    _glLightCount         = glGetUniformLocation(handle, "lightcount");
    _glSceneAmbient       = glGetUniformLocation(handle, "sceneambient");

    _glCameraPosition     = glGetUniformLocation(handle, "camerapos_ws");

    _ambientlight = glm::vec3(0.2, 0.2, 0.2);
}


// Returns a new unique id to represent an object in the scene
objectid_t Scene::getNewObjectId()
{
    _objects.push_back(nullptr);
    return _objects.size()-1;
}

// Adds an object with a given id to the scene
void Scene::insertObject(object_t obj)
{
    if (obj->getId() >= _objects.size()) {
        cerr << __FUNCTION__ << ": object id is invalid or out of range (did you first set obj->id with getNewObjectId()? )\n";
        return;
    } else if (_objects[obj->getId()] != nullptr) {
        cerr << __FUNCTION__ << ": inserting object into non-NULL bin (this indicates a bug)\n";
        return;
    }

    _objects[obj->getId()] = obj;
}

// Remove an object from the scene.  The object must have a valid objectid
void Scene::removeObject(object_t obj)
{
    if (obj->getId() >= _objects.size()) {
        cerr << __FUNCTION__ << ": object id is invalid or out of range\n";
        return;
    } else if (_objects[obj->getId()] == nullptr) {
        cerr << __FUNCTION__ << ": object was already removed from the scene\n";
        return;
    }

    _objects[obj->getId()] = nullptr;
}

void Scene::insertParticleSystem(std::shared_ptr<ParticleSystem> ps)
{
    _particlesystems.push_back(ps);
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

    glm::mat4 viewmatrix    = glm::lookAt(eyeposition, lookdirection, updirection);
    glm::mat4 viewinvmatrix = glm::inverseTranspose(viewmatrix);

    // pass our transformation matricies to the shaders
    glUniformMatrix4fv(_glViewMatrix, 1, GL_FALSE, glm::value_ptr(viewmatrix));
    glUniformMatrix4fv(_glViewInverseMatrix, 1, GL_FALSE, glm::value_ptr(viewinvmatrix));
    glUniformMatrix4fv(_glProjectionMatrix, 1, GL_FALSE, glm::value_ptr(projectionmatrix));

    // don't allow more than 10 lights for now...
    if (_lights.size() > 5) {
        cerr << "Warning: Limiting light sources to 5\n\n";
        _lights.resize(5);
    }

    // tell the shaders about lighting inforamtion
    vector<glm::vec4> lightpos;
    vector<glm::vec4> lightdiffuse;
    vector<glm::vec4> lightspecular;
    vector<glm::vec3> lightattn;
    vector<GLfloat>   lightspotcutoff;
    vector<GLfloat>   lightspotexponent;
    vector<glm::vec3> lightspotdir;
    for (size_t i=0; i<_lights.size(); i++) {
        const Light &light = _lights[i];

        lightpos     .push_back(glm::vec4(light.getPosition(), 1.0));
        lightdiffuse .push_back(glm::vec4(light.getDiffuse(), 1.0));
        lightspecular.push_back(glm::vec4(light.getSpecular(), 1.0));
        lightattn    .push_back(glm::vec3(light.getConstantAttenuation(),
                                          light.getLinearAttenuation(),
                                          light.getQuadraticAttenuation()));
        lightspotcutoff  .push_back(light.getSpotCutoff());
        lightspotexponent.push_back(light.getSpotExponent());
        lightspotdir     .push_back(light.getSpotDirection());
    }

    if (_lights.size() > 0) {
        glUniform4fv(_glLightPosition,      5, glm::value_ptr(lightpos[0]));
        glUniform4fv(_glLightDiffuse,       5, glm::value_ptr(lightdiffuse[0]));
        glUniform4fv(_glLightSpecular,      5, glm::value_ptr(lightspecular[0]));
        glUniform3fv(_glLightAttenuation,   5, glm::value_ptr(lightattn[0]));
        glUniform1fv(_glLightSpotCutoff,    5, &lightspotcutoff[0]);
        glUniform1fv(_glLightSpotExponent,  5, &lightspotexponent[0]);
        glUniform3fv(_glLightSpotDirection, 5, glm::value_ptr(lightspotdir[0]));
    }

    glUniform1i(_glLightCount, (GLint)_lights.size());
    glUniform3f(_glSceneAmbient, _ambientlight.r, _ambientlight.g, _ambientlight.b);
    glUniform4f(_glCameraPosition, eyeposition.x, eyeposition.y, eyeposition.z, 1.0);

    // now render each object
    for (size_t i=0; i<_objects.size(); i++) {
        if (_objects[i] != nullptr)
            _objects[i]->render(_glModelMatrix, _glModelInvTranspMatrix);
    }

    // now render each particle system
    for (size_t i=0; i<_particlesystems.size(); i++) {
        if (_particlesystems[i] != nullptr)
            _particlesystems[i]->render();
    }
}
