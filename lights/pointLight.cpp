#include "pointLight.h"

void PointLight::ChangePositionToObject()
{
    if (object != nullptr)
        Position = this->object->GetWorldPosition();
}
void PointLight::BindToObject(const std::shared_ptr<Drawable> &ptr) { object = std::make_unique<Drawable>(*ptr); }

void PointLight::UseInShader(const Shader &shader)
{
    std::string identifire = "point_light[" + std::to_string(Number) + "]";
    shader.Use();
    shader.SetVec3(identifire + ".position", Position);
    shader.SetFloat(identifire + ".constant", Constant);
    shader.SetFloat(identifire + ".linear", Linear);
    shader.SetFloat(identifire + ".quadratic", Quadratic);
    shader.SetVec3(identifire + ".ambient", Ambient);
    shader.SetVec3(identifire + ".diffuse", Diffuse);
    shader.SetVec3(identifire + ".specular", Specular);
}
PointLight::PointLight(
    glm::vec3 position, float constant, float linear, float quadratic, glm::vec3 ambient, glm::vec3 diffuse,
    glm::vec3 specular, unsigned int number
): Position(position), Constant(constant), Linear(linear), Quadratic(quadratic), Ambient(ambient), Diffuse(diffuse),
Specular(specular), Number(number){}
