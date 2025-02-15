//
// Created by Mykhailo_Shamrai on 11.02.2025.
//
#include "spotLight.h"

SpotLight::SpotLight(glm::vec3 position, glm::vec3 direction, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular,
    float cutOff, float outerCutOff, unsigned int number):
    Position(position), Direction(direction), DirectionAfter(direction),
    Ambient(ambient), Diffuse(diffuse), Specular(specular), cutOff(cutOff), Number(number),
    outerCutOff(outerCutOff){}

void SpotLight::ChangePositionToObject()
{
    // In spotLight we need have all transformations
    // of the object for which we are bounded to dir vector
    if (object != nullptr)
    {
        this->Position = this->object->GetWorldPosition();
        glm::vec3 rot    = this->object->GetRotation();
        glm::mat4 matrix = glm::mat4(1.0f);
        //matrix           = glm::translate(matrix, pos);
        float xAngle     = rot.x;
        float yAngle     = rot.y;
        float zAngle     = rot.z;
        glm::vec3 xAxis(1.0f, 0.0f, 0.0f);
        glm::vec3 yAxis(0.0f, 1.0f, 0.0f);
        glm::vec3 zAxis(0.0f, 0.0f, 1.0f);
        matrix               = glm::rotate(matrix, glm::radians(xAngle), xAxis);
        matrix               = glm::rotate(matrix, glm::radians(yAngle), yAxis);
        matrix               = glm::rotate(matrix, glm::radians(zAngle), zAxis);
        this->DirectionAfter = glm::normalize(glm::vec3(matrix * glm::vec4(Direction, 1.0f)));
    }
}

void SpotLight::BindToObject(const std::shared_ptr<Drawable> &ptr)
{
    object = ptr;
}

void SpotLight::UseInShader(const Shader &shader)
{
    std::string identifier = "spotLights[" + std::to_string(Number) + "]";
    shader.Use();
    shader.SetVec3(identifier + ".position", Position);
    shader.SetVec3(identifier + ".direction", DirectionAfter);

    shader.SetVec3(identifier + ".ambient", Ambient);
    shader.SetVec3(identifier + ".diffuse", Diffuse);
    shader.SetVec3(identifier + ".specular", Specular);

    shader.SetFloat(identifier + ".cutOff", glm::cos(glm::radians(cutOff)));
    shader.SetFloat(identifier + ".outerCutOff", glm::cos(glm::radians(outerCutOff)));
}

void SpotLight::ChangeDirVector(glm::vec3 direction)
{
    this->Direction = glm::normalize(direction);
}
