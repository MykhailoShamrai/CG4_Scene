#ifndef DRAWABLE_H
#define DRAWABLE_H
#include "shader.h"

// Class that represents the all objects that are drawable. It's something like
// interface, all objects like models or my custom objects should implement it.

class Drawable
{
public:
    // Three numbers for position in world space
    glm::vec3 WorldPosition = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 Scale = glm::vec3(1.0f, 1.0f, 1.0f);
    // First element of vector -> rotation around x
    // Second element -> rotation around y
    // Third element -> rotation around z
    glm::vec3 Rotation = glm::vec3(0.0f, 0.0f, 0.0f);

    virtual ~Drawable();
    virtual void Draw(const Shader &shader);
protected:
    glm::mat4 CreateModelMatrix();
    void SetTransformationsForDrawable(const Shader &shader);
};

#endif //DRAWABLE_H
