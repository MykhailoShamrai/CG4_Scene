#include "pointLight.h"

void PointLight::ChangePositionToObject()
{
    if (object != nullptr)
        Position = this->object->GetWorldPosition();
}
void PointLight::BindToObject(const std::shared_ptr<Drawable> &ptr) { object = ptr; }

void PointLight::UseInShader(const Shader &shader)
{
    std::string identifier = "pointLights[" + std::to_string(Number) + "]";
    shader.Use();
    shader.SetVec3(identifier + ".position", Position);
    shader.SetFloat(identifier + ".constant", Constant);
    shader.SetFloat(identifier + ".linear", Linear);
    shader.SetFloat(identifier + ".quadratic", Quadratic);
    shader.SetVec3(identifier + ".ambient", Ambient);
    shader.SetVec3(identifier + ".diffuse", Diffuse);
    shader.SetVec3(identifier + ".specular", Specular);
}
PointLight::PointLight(
    glm::vec3 position, float constant, float linear, float quadratic, glm::vec3 ambient, glm::vec3 diffuse,
    glm::vec3 specular, unsigned int number
): Position(position), Constant(constant), Linear(linear), Quadratic(quadratic), Ambient(ambient), Diffuse(diffuse),
Specular(specular), Number(number){}
