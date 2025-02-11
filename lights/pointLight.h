#ifndef POINTLIGHT_H
#define POINTLIGHT_H
#include <memory>

#include "BindLight.h"
#include "drawable.h"
#include "glm/detail/type_vec.hpp"

struct PointLight: BindLight
{
    glm::vec3 Position;

    float Constant;
    float Linear;
    float Quadratic;

    glm::vec3 Ambient;
    glm::vec3 Diffuse;
    glm::vec3 Specular;

    unsigned int Number = 0;
    std::unique_ptr<Drawable> object = nullptr;
    void ChangePositionToObject() override;
    void BindToObject(const std::shared_ptr<Drawable>& ptr) override;
    void UseInShader(const Shader &shader) override;
    PointLight(glm::vec3 position, float constant, float linear, float quadratic,
        glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, unsigned int number);
};

#endif //POINTLIGHT_H
