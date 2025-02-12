#ifndef CAMERA_H
#define CAMERA_H
#include <glad/glad.h>

#include "drawable.h"
#include "glm/detail/type_mat.hpp"
#include "glm/detail/type_vec3.hpp"

enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

const float SPEED       =  10.0f;
const float SENSITIVITY =  0.1f;
const float ZOOM        =  45.0f;

class Camera
{
public:
    float MovementSpeed;
    float MouseSensitivity;
    float Zoom;
    std::shared_ptr<Drawable> BindObject = nullptr;

    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
        float yaw = -90.0f, float pitch = 0.0f);
    glm::mat4 GetView();
    void updateCameraTarget(glm::vec3 target);
    void ProcessKeyboard(Camera_Movement direction, float deltaTime);
    void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true);
    void ProcessMouseScroll(float yoffset);
    bool GetMovable() const { return isMovable; }
    void SetMovable(const bool &val) { isMovable = val; }
    bool GetThirdPerson() const { return isThirdPerson; }
    void SetThirdPerson(const bool &val) { isThirdPerson = val; }
    bool GetIsFindingObject() const { return isFindingObject; }
    void SetIsFindingObject(const bool &val) { isFindingObject = val; }
    void ProcessCamera();
    void ChangeCameraThirdPerson();

    glm::vec3 GetCameraPosition() { return cameraPosition; }
private:
    glm::vec3 cameraPosition;
    // Direction
    glm::vec3 cameraFront;
    glm::vec3 cameraUp;
    glm::vec3 cameraRight;
    glm::vec3 worldUp;
    bool isMovable;
    bool isThirdPerson;
    bool isFindingObject;

    // Function for finding front vector
    void updateCameraRightUp();
    void updateCameraFrontWithYawPitch();
    float yaw;
    float pitch;
};

#endif //CAMERA_H
