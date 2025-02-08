//
// Created by Mykhailo_Shamrai on 08.02.2025.
//
#include "drawable.h"

#include <stdexcept>

Drawable::~Drawable() = default;
void Drawable::Draw(const Shader &shader) { throw std::logic_error("Drawable::Draw() is not implemented"); }
