#ifndef SPOTLIGHT_H
#define SPOTLIGHT_H
#include <memory>

#include "bindedLight.h"
#include "dirLight.h"
#include "drawable.h"
#include "glm/detail/type_vec.hpp"
#include "glm/gtc/matrix_transform.hpp"

struct SpotLight: public BindedLight
{
    glm::vec3 Position;
    glm::vec3 Direction;
    glm::vec3 DirectionAfter;

    glm::vec3 Ambient;
    glm::vec3 Diffuse;
    glm::vec3 Specular;
    float cutOff;
    // Object with which a light can be bind
    std::unique_ptr <Drawable> object = nullptr;

    void ChangePositionToObject() override;
};


#endif //SPOTLIGHT_H
