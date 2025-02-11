#ifndef POINTLIGHT_H
#define POINTLIGHT_H
#include <memory>

#include "bindedLight.h"
#include "drawable.h"
#include "glm/detail/type_vec.hpp"

struct PointLight: BindedLight
{
    glm::vec3 Position;

    glm::vec3 Constant;
    glm::vec3 Linear;
    glm::vec3 Quadratic;

    glm::vec3 Ambient;
    glm::vec3 Diffuse;
    glm::vec3 Specular;

    std::unique_ptr<Drawable> object = nullptr;
    void ChangePositionToObject() override
    {
        if (object != nullptr)
            Position = this->object->GetWorldPosition();
    };
};

#endif //POINTLIGHT_H
