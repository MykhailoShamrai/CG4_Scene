//
// Created by Mykhailo_Shamrai on 08.02.2025.
//
#include "drawable.h"

#include <stdexcept>

#include "glm/gtc/matrix_transform.hpp"

float Drawable::GetXPosition() const { return worldPosition.x; }
float Drawable::GetYPosition() const { return worldPosition.y; }
float Drawable::GetZPosition() const { return worldPosition.z; }
float Drawable::GetXRotation() const { return rotation.x; }
float Drawable::GetYRotation() const { return rotation.y; }
float Drawable::GetZRotation() const { return rotation.z; }
float Drawable::GetScale() const { return scale.x; }
glm::vec3 Drawable::GetWorldPosition() const { return worldPosition; }
glm::vec3 Drawable::GetOldPosition() const { return oldPosition; }
glm::vec3 Drawable::GetRotation() const { return rotation; }
void Drawable::SetOldPos(const glm::vec3 &pos) { oldPosition = pos; }

void Drawable::SetXPosition(float x)
{
    oldPosition.x = worldPosition.x;
    worldPosition.x = x;
}

void Drawable::SetYPosition(float y)
{
    oldPosition.y = worldPosition.y;
    worldPosition.y = y;
}

void Drawable::SetZPosition(float z)
{
    oldPosition.z = worldPosition.z;
    worldPosition.z = z;
}

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

void Drawable::Draw(const std::unordered_map<std::string, Shader> &shaders)
{
    throw std::logic_error("Drawable::Draw() is not implemented");
}

void Drawable::SetSpecularAndShininess(const glm::vec3 &specular, const float &shininess)
{
    customMaterial.Specular  = specular;
    customMaterial.Shininess = shininess;
}
void Drawable::Animate(float timeElapsed)
{
    if (animator != nullptr)
        animator->Animate(timeElapsed, *this);
}

void Drawable::SetAnimated(const std::shared_ptr<AnimatorBase> &animator)
{
    this->animator = animator;
}

void Drawable::SetCustomMaterial(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float shininess)
{
    customMaterial = Material{ ambient, diffuse, specular, shininess, true };
}

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

void Drawable::setTransformationsForDrawable(const std::unordered_map<std::string, Shader> &shaders)
{
    glm::mat4 modelMatrix = createModelMatrix();
    glm::mat3 normalMatrix = glm::mat3(glm::transpose(glm::inverse(modelMatrix)));
    for (const auto &shader : shaders)
    {
        shader.second.Use();
        shader.second.SetMat4("model", modelMatrix);
        shader.second.SetMat3("normalMatrix", normalMatrix);
    }
}
