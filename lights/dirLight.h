#ifndef DIRLIGHT_H
#define DIRLIGHT_H
#include "glm/detail/type_vec.hpp"
#include "glm/detail/type_vec3.hpp"

struct DirLight
{
    glm::vec3 Direction;

    glm::vec3 Ambient;
    glm::vec3 Diffuse;
    glm::vec3 Specular;
};

#endif //DIRLIGHT_H
