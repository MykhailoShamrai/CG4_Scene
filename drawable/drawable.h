#ifndef DRAWABLE_H
#define DRAWABLE_H
#include "shader.h"

// Class that represents the all objects that are drawable. It's something like
// interface, all objects like models or my custom objects should implement it.

class Drawable
{
public:
    float GetXPosition() const;
    float GetYPosition() const;
    float GetZPosition() const;
    float GetXRotation() const;
    float GetYRotation() const;
    float GetZRotation() const;
    float GetScale() const;
    glm::vec3 GetWorldPosition() const;
    glm::vec3 GetRotation() const;

    void SetXPosition(float x);
    void SetYPosition(float y);
    void SetZPosition(float z);
    void SetXRotation(float angle);
    void SetYRotation(float angle);
    void SetZRotation(float angle);
    void SetScale(float sc);
    virtual ~Drawable();
    virtual void Draw(const Shader &shader);

protected:
    // Three numbers for position in world space
    glm::vec3 worldPosition = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 scale         = glm::vec3(1.0f, 1.0f, 1.0f);
    glm::vec3 rotation      = glm::vec3(0.0f, 0.0f, 0.0f);
    // First element of vector -> rotation around x
    // Second element -> rotation around y
    // Third element -> rotation around z
    glm::mat4 createModelMatrix();
    void setTransformationsForDrawable(const Shader &shader);
};

#endif  // DRAWABLE_H
