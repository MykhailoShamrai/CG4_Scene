//
// Created by Mykhailo_Shamrai on 08.02.2025.
//
#include "drawable.h"

#include <stdexcept>

#include "glm/gtc/matrix_transform.hpp"

Drawable::~Drawable() = default;
void Drawable::Draw(const Shader &shader) { throw std::logic_error("Drawable::Draw() is not implemented"); }
glm::mat4 Drawable::CreateModelMatrix()
{
    glm::mat4 modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, WorldPosition);
    // Values in angles
    float xAngle = Rotation.x;
    float yAngle = Rotation.y;
    float zAngle = Rotation.z;
    glm::vec3 xAxis(1.0f, 0.0f, 0.0f);
    glm::vec3 yAxis(0.0f, 1.0f, 0.0f);
    glm::vec3 zAxis(0.0f, 0.0f, 1.0f);
    modelMatrix = glm::rotate(modelMatrix, glm::radians(xAngle), xAxis);
    modelMatrix = glm::rotate(modelMatrix, glm::radians(yAngle), yAxis);
    modelMatrix = glm::rotate(modelMatrix, glm::radians(zAngle), zAxis);
    modelMatrix = glm::scale(modelMatrix, Scale);
    return modelMatrix;
}

void Drawable::SetTransformationsForDrawable(const Shader &shader)
{
    glm::mat4 modelMatrix = CreateModelMatrix();
    // TODO: Also pass an inversed matrix for normal vectors
    glm::mat3 normalMatrix = glm::mat3(glm::transpose(glm::inverse(modelMatrix)));
    shader.SetMat4("model", modelMatrix);
    shader.SetMat3("normalMatrix", normalMatrix);
}
