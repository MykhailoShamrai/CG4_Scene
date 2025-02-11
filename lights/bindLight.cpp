#include "bindLight.h"

#include <stdexcept>

BindLight::~BindLight() = default;

void BindLight::ChangePositionToObject()
{
    throw std::logic_error("BindLight::ChangePositionToObject() is not implemented");
}

void BindLight::BindToObject(const std::shared_ptr<Drawable> &ptr)
{
    throw std::logic_error("BindLight::BindToObject() is not implemented");
}

void BindLight::UseInShader(const Shader &shader)
{
    throw std::logic_error("BindLight::BindToObject() is not implemented");
}


