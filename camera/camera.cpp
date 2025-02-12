#include "camera.h"

#include <ios>

#include "glm/gtc/matrix_transform.hpp"


glm::mat4 Camera::GetView()
{
    return glm::lookAt(cameraPosition, cameraPosition + cameraFront, worldUp);
}

void Camera::updateCameraRightUp()
{
    cameraRight = glm::normalize(glm::cross(cameraFront, worldUp));
    cameraUp = glm::normalize(glm::cross(cameraRight, worldUp));
}

void Camera::updateCameraTarget(glm::vec3 target)
{
    cameraFront = target - cameraPosition;

    updateCameraRightUp();
}

void Camera::updateCameraFrontWithYawPitch()
{
    glm::vec3 front;

    front.x = cosf(glm::radians(pitch)) * cosf(glm::radians(yaw));
    front.y = sinf(glm::radians(pitch));
    front.z = sinf(glm::radians(yaw)) * cosf(glm::radians(pitch));
    cameraFront = glm::normalize(front);

    updateCameraRightUp();
}

// processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
void Camera::ProcessKeyboard(Camera_Movement direction, float deltaTime)
{
    float velocity = MovementSpeed * deltaTime;
    if (direction == FORWARD)
        cameraPosition += cameraFront * velocity;
    if (direction == BACKWARD)
        cameraPosition -= cameraFront * velocity;
    if (direction == LEFT)
        cameraPosition -= cameraRight * velocity;
    if (direction == RIGHT)
        cameraPosition += cameraRight * velocity;
}

// processes input received from a mouse input system. Expects the offset value in both the x and y direction.
void Camera::ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch)
{
    xoffset *= MouseSensitivity;
    yoffset *= MouseSensitivity;

    yaw += xoffset;
    pitch += yoffset;

    // make sure that when pitch is out of bounds, screen doesn't get flipped
    if (constrainPitch)
    {
        if (pitch > 89.0f)
            pitch = 89.0f;
        if (pitch < -89.0f)
            pitch = -89.0f;
    }

    updateCameraFrontWithYawPitch();
}

Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch): yaw(yaw), pitch(pitch),
    cameraFront(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
{
    cameraPosition = position;
    worldUp = up;
    updateCameraFrontWithYawPitch();
}

// processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
void Camera::ProcessMouseScroll(float yoffset)
{
    Zoom -= (float)yoffset;
    if (Zoom < 1.0f)
        Zoom = 1.0f;
    if (Zoom > 45.0f)
        Zoom = 45.0f;
}

void Camera::ProcessCamera()
{
    if (GetThirdPerson())
    {
        ChangeCameraThirdPerson();
    }
    else if (GetIsFindingObject())
    {

    }

}
void Camera::ChangeCameraThirdPerson()
{
    static float offsetUp = 100.0f;
    static float offsetBack = 30.0f;
    static float offsetFront = 3.0f;
    if (GetThirdPerson() && BindObject != nullptr)
    {
        glm::vec3 objPos = BindObject->GetWorldPosition();
        glm::vec3 up = worldUp;
        glm::vec3 objectForward = glm::normalize(BindObject->GetWorldPosition() - BindObject->GetOldPosition());
        glm::vec3 back = - objectForward;
        glm::vec3 tmpPos = objPos + back * offsetBack + offsetUp * up;
        cameraPosition = tmpPos;
        updateCameraTarget(objPos + offsetFront * objectForward);
    }
}
