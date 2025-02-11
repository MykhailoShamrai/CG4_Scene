#ifndef SPOTLIGHT_H
#define SPOTLIGHT_H
#include <memory>

#include "bindLight.h"
#include "dirLight.h"
#include "drawable.h"
#include "glm/detail/type_vec.hpp"
#include "glm/gtc/matrix_transform.hpp"

struct SpotLight: public BindLight
{
    glm::vec3 Position;
    glm::vec3 Direction;
    glm::vec3 DirectionAfter;

    glm::vec3 Ambient;
    glm::vec3 Diffuse;
    glm::vec3 Specular;
    float cutOff;

    unsigned int Number = 0;
    // Object with which a light can be bind
    std::shared_ptr <Drawable> object = nullptr;
    SpotLight(glm::vec3 position,
        glm::vec3 direction,
        glm::vec3 ambient,
        glm::vec3 diffuse,
        glm::vec3 specular,
        float cutOff,
        unsigned int number);

    void ChangePositionToObject() override;
    void BindToObject(const std::shared_ptr<Drawable>& ptr) override;
    void UseInShader(const Shader &shader) override;
};


#endif //SPOTLIGHT_H
