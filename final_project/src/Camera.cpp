#define GLM_SWIZZLE
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include <cmath>
#include "Camera.hpp"
#include "Scene.hpp"


Camera::Camera()
    : _scene(NULL)
{
    reset();
}

Camera::Camera(Scene *scene)
    : _scene(scene)
{
    reset();
}


// Reset the camera back to its defaults
void Camera::reset()
{
    _lookdirection = glm::normalize(glm::vec3(0.0, 0.0,-1.0));
    _updirection   = glm::normalize(glm::vec3(0.0, 1.0, 0.0));
    _position      = glm::vec3(0.0, 0.0, 0.3);

    _fieldofview   = 45.0f; // frustrum viewing apeture
    _aspectratio   = 16.0f/10.0f;
    _nearfieldclip = 0.01; // clip anything closer than this
    _farfieldclip  = 10.0; // clip anything farther than this
    _projection    = glm::perspective(_fieldofview, _aspectratio, _nearfieldclip, _farfieldclip);
    //_projection    = glm::ortho(-1.2f, 1.2f, -0.75f, 0.75f, _nearfieldclip, _farfieldclip);
}


void Camera::setScene(Scene *scene)
{
    _scene = scene;
}


// Set the camera to an arbitrary location without changing orientation
void Camera::setPosition(glm::vec3 pos)
{
    _position = pos;
}

// Set the orientation of the camera without changing its position
void Camera::lookAt(glm::vec3 look, glm::vec3 up)
{
    _lookdirection = glm::normalize(look);
    _updirection   = glm::normalize(up);
}

void Camera::setYawFromLook(glm::vec3 look, float angle)
{
    glm::vec4 look4(look, 0.0);
    glm::mat4 matrix = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0.0, 1.0, 0.0)/*_updirection*/);
    look4 = matrix * look4;
    //lookAt(look4.xyz(), _updirection);
    _lookdirection = look4.xyz();
}


void Camera::setPitchFromLook(glm::vec3 look, float angle)
{
    glm::vec4 up(glm::vec3(0.0, 1.0, 0.0), 0.0);
    glm::vec4 look4(look, 0.0);

    // Move to the origin
    glm::mat4 matrix = glm::translate(glm::mat4(1.0f), -_position);
    // Pitch the camera
    matrix = glm::rotate(matrix, angle, glm::cross(look, glm::vec3(0.0, 1.0, 0.0)));
    // Move it back to it's original spot
    matrix = glm::translate(matrix, _position);
    
    look4 = matrix * look4;
    up    = matrix * up;

    //lookAt(look4.xyz(), up.xyz());

    _lookdirection = look4.xyz();
    _updirection   = up.xyz();
}


void Camera::moveX(float units)
{
    float abs_units = (float)fabs(units);
    glm::vec3 direction = glm::normalize(glm::cross(_lookdirection, _updirection)); 
    glm::vec3 shiftvector = abs_units * (units<0.0? -direction: direction);
    _position += shiftvector; 
    _lookdirection = glm::normalize(_lookdirection + shiftvector);
}

void Camera::moveY(float units)
{
    float abs_units = (float)fabs(units);
    glm::vec3 shiftvector = abs_units * (units<0.0? -_updirection: _updirection);
    _position += shiftvector;
    _lookdirection = glm::normalize(_lookdirection + shiftvector);
}

void Camera::moveZ(float units)
{
    float abs_units = (float)fabs(units);
    glm::vec3 shiftvector = abs_units * (units<0.0? -_lookdirection: _lookdirection);
    _position += shiftvector;
    _lookdirection = glm::normalize(_lookdirection + shiftvector);
}

// Translate the camera along X/Y/Z
void Camera::translate(glm::vec3 v)
{
    glm::vec4 pos(_position, 1.0);
    glm::vec4 look(_lookdirection, 1.0);
    glm::mat4 matrix = glm::translate(glm::mat4(1.0f), v);
    pos = matrix * pos;
    look = matrix * look;

    _position      = pos.xyz();
    _lookdirection = look.xyz();
}

// Roll the camera along the look axis
void Camera::roll(float theta)
{
    glm::vec4 up(_updirection, 0.0);
    glm::mat4 matrix = glm::rotate(glm::mat4(1.0f), theta, _lookdirection);
    up = matrix * up;
    _updirection = up.xyz();
}

// Pitch the camera along the axis orthogonal to the up and look vectors
void Camera::pitch(float theta)
{
    glm::vec4 look(_lookdirection, 0.0);
    glm::vec4 up(_updirection, 0.0);

    // Move to the origin
    glm::mat4 matrix = glm::translate(glm::mat4(1.0f), -_position);
    // Pitch the camera
    matrix = glm::rotate(matrix, theta, glm::cross(_lookdirection, _updirection));
    // Move it back to it's original spot
    matrix = glm::translate(matrix, _position);
    
    look = matrix * look;
    up   = matrix * up;

    _lookdirection = look.xyz();
    _updirection   = up.xyz();
}

// Yaw the camera along the up vector
void Camera::yaw(float theta)
{
    glm::vec4 look(_lookdirection, 0.0);
    glm::mat4 matrix = glm::rotate(glm::mat4(1.0f), theta, _updirection);
    look = matrix * look;
    _lookdirection = look.xyz();
}

// Sets the perspective of the camera
void Camera::setPerspective(float fieldOfViewDegrees, float aspectRatio, float nearclipdistance, float farclipdistance)
{
    _fieldofview   = fieldOfViewDegrees;
    _aspectratio   = aspectRatio;
    _nearfieldclip = nearclipdistance;
    _farfieldclip  = farclipdistance;
    _projection = glm::perspective(_fieldofview, _aspectratio, _nearfieldclip, _farfieldclip);
}


void Camera::setFieldOfView(float degrees)
{ 
    _fieldofview = degrees;
    _projection = glm::perspective(_fieldofview, _aspectratio, _nearfieldclip, _farfieldclip);
}


void Camera::setAspectRatio(float ratio)
{ 
    _aspectratio = ratio;
    _projection = glm::perspective(_fieldofview, _aspectratio, _nearfieldclip, _farfieldclip);
}


void Camera::setNearFieldClipDistance(float distance)
{ 
    _nearfieldclip = distance;
    _projection = glm::perspective(_fieldofview, _aspectratio, _nearfieldclip, _farfieldclip);
}


void Camera::setFarFieldClipDistance(float distance)
{ 
    _farfieldclip = distance;
    _projection = glm::perspective(_fieldofview, _aspectratio, _nearfieldclip, _farfieldclip);
}


// Render the scene with the current camera settings
void Camera::render()
{
    if (_scene)
        _scene->render(_position, _lookdirection, _updirection, _projection);
}


