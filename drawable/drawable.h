#ifndef DRAWABLE_H
#define DRAWABLE_H
#include <memory>
#include <unordered_map>

#include "../animator/animatorBase.h"
#include "shader.h"

struct Material {
    glm::vec3 Ambient;
    glm::vec3 Diffuse;
    glm::vec3 Specular;
    float Shininess = -1.0f;
    bool IsCustom = false;
};

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
    virtual void Draw(const std::unordered_map<std::string, Shader> &shaders);
    virtual void SetCustomMaterial(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float shininess);
    virtual void SetSpecularAndShininess(const glm::vec3 &specular, const float &shininess);
    void Animate(float timeElapsed);
    void SetAnimated(const std::shared_ptr<AnimatorBase> &animator);
protected:
    // Three numbers for position in world space
    glm::vec3 worldPosition = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 scale         = glm::vec3(1.0f, 1.0f, 1.0f);
    glm::vec3 rotation      = glm::vec3(0.0f, 0.0f, 0.0f);
    Material customMaterial;
    // First element of vector -> rotation around x
    // Second element -> rotation around y
    // Third element -> rotation around z
    std::shared_ptr<AnimatorBase> animator = nullptr;
    glm::mat4 createModelMatrix();
    void setTransformationsForDrawable(const std::unordered_map<std::string, Shader> &shaders);
};

#endif  // DRAWABLE_H
