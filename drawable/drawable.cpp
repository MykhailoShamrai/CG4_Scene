//
// Created by Mykhailo_Shamrai on 08.02.2025.
//
#include "drawable.h"

#include <stdexcept>

#include "glm/gtc/matrix_transform.hpp"

void Drawable::SetXPosition(float x) { worldPosition.x = x; }
void Drawable::SetYPosition(float y) { worldPosition.y = y; }
void Drawable::SetZPosition(float z) { worldPosition.z = z; }
void Drawable::SetXRotation(float angle) { rotation.x = angle; }
void Drawable::SetYRotation(float angle) { rotation.y = angle; }
void Drawable::SetZRotation(float angle) { rotation.z = angle; }
void Drawable::SetScale(float sc)
{
    scale.x = sc;
    scale.y = sc;
    scale.z = sc;
}
Drawable::~Drawable() = default;
void Drawable::Draw(const Shader &shader) { throw std::logic_error("Drawable::Draw() is not implemented"); }
glm::mat4 Drawable::createModelMatrix()
{
    glm::mat4 modelMatrix = glm::mat4(1.0f);
    modelMatrix           = glm::translate(modelMatrix, worldPosition);
    // Values in angles
    float xAngle = rotation.x;
    float yAngle = rotation.y;
    float zAngle = rotation.z;
    glm::vec3 xAxis(1.0f, 0.0f, 0.0f);
    glm::vec3 yAxis(0.0f, 1.0f, 0.0f);
    glm::vec3 zAxis(0.0f, 0.0f, 1.0f);
    modelMatrix = glm::rotate(modelMatrix, glm::radians(xAngle), xAxis);
    modelMatrix = glm::rotate(modelMatrix, glm::radians(yAngle), yAxis);
    modelMatrix = glm::rotate(modelMatrix, glm::radians(zAngle), zAxis);
    modelMatrix = glm::scale(modelMatrix, scale);
    return modelMatrix;
}

void Drawable::setTransformationsForDrawable(const Shader &shader)
{
    glm::mat4 modelMatrix = createModelMatrix();
    // TODO: Also pass an inversed matrix for normal vectors
    glm::mat3 normalMatrix = glm::mat3(glm::transpose(glm::inverse(modelMatrix)));
    shader.SetMat4("model", modelMatrix);
    shader.SetMat3("normalMatrix", normalMatrix);
}
