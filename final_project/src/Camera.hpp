#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "glm/glm.hpp"
class Scene;



class Camera {
public:
    Camera();
    Camera(Scene *scene);

    void setScene(Scene *scene);

    // resets the camera to defaults
    void reset();

    const glm::vec3 &getLookDirection() const { return _lookdirection; }

    // Functions to move/direct the camera
    void setPosition(glm::vec3 pos);
    void lookAt(glm::vec3 look, glm::vec3 up);
    void setYawFromLook(glm::vec3 look, float angle);
    void setPitchFromLook(glm::vec3 look, float angle);

    void moveX(float units);
    void moveY(float units);
    void moveZ(float units);

    void translate(glm::vec3 v);
    void roll(float theta);
    void pitch(float theta);
    void yaw(float theta);

    // Set the perspective of the camera and its clipping distances
    void setPerspective(float fieldOfViewDegrees, float aspectRatio, float nearclip, float farclip);

    // Individually set any of the perspective fields
    void setFieldOfView(float degrees);
    void setAspectRatio(float ratio);
    void setNearFieldClipDistance(float distance);
    void setFarFieldClipDistance(float distance);

    void render();

private:
    Scene *_scene; 

    glm::vec3 _lookdirection;
    glm::vec3 _position;
    glm::vec3 _updirection;

    glm::mat4 _projection;
    float _fieldofview;    // frustrum viewing apeture
    float _aspectratio; 
    float _nearfieldclip;  // clip anything closer than this
    float _farfieldclip;   // clip anything farther than this
};

#endif

