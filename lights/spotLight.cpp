//
// Created by Mykhailo_Shamrai on 11.02.2025.
//
#include "spotLight.h"

void SpotLight::ChangePositionToObject()
{
    // In spotLight we need have all transformations
    // of the object for which we are bounded to dir vector
    if (object != nullptr)
    {
        glm::vec3 pos    = this->object->GetWorldPosition();
        glm::vec3 rot    = this->object->GetRotation();
        glm::mat4 matrix = glm::mat4(1.0f);
        matrix           = glm::translate(matrix, pos);
        float xAngle     = rot.x;
        float yAngle     = rot.y;
        float zAngle     = rot.z;
        glm::vec3 xAxis(1.0f, 0.0f, 0.0f);
        glm::vec3 yAxis(0.0f, 1.0f, 0.0f);
        glm::vec3 zAxis(0.0f, 0.0f, 1.0f);
        matrix = glm::rotate(matrix, glm::radians(xAngle), xAxis);
        matrix = glm::rotate(matrix, glm::radians(yAngle), yAxis);
        matrix = glm::rotate(matrix, glm::radians(zAngle), zAxis);
        this->DirectionAfter = glm::vec3(matrix * glm::vec4(Direction, 1.0f));
    }
}