#ifndef SCENE_H
#define SCENE_H
#include <map>
#include <memory>
#include <string>
#include <unordered_map>

#include "../camera/camera.h"
#include "../lights/bindLight.h"
#include "../lights/dirLight.h"
#include "drawable.h"

class Scene
{
public:
    // Map of positions in world space
    std::pmr::unordered_map<std::string, std::shared_ptr<Drawable>> Drawables =
        std::pmr::unordered_map<std::string, std::shared_ptr<Drawable>>();
    std::shared_ptr<Drawable> ChosenObject;
    std::string ChosenObjectName;

    // Map of cameras on scene
    std::pmr::unordered_map<std::string, std::shared_ptr<Camera>> Cameras =
        std::pmr::unordered_map<std::string, std::shared_ptr<Camera>>();
    std::shared_ptr<Camera> CurrentCamera;
    std::string CurrentCameraName;

    // Map of PointLights on scene
    std::pmr::unordered_map<std::string, std::shared_ptr<BindLight>> Lights;
    std::shared_ptr<BindLight> CurrentLight;
    std::string CurrentLightName;

    // Global Direction light
    DirLight DirectionalLight = DirLight
    {
        glm::vec3(0.0f, -1.0f, 0.0f),
        glm::vec3(0.2f, 0.2f, 0.2f),
        glm::vec3(0.5f, 0.5f, 0.5f),
        glm::vec3(1.0f, 1.0f, 1.0f)
    };

    void AddSpotLight(const std::string& lightName,
        const glm::vec3& position,
        const glm::vec3& direction,
        const glm::vec3& ambient,
        const glm::vec3& diffuse,
        const glm::vec3& specular,
        const float& cutOff,
        const float& outerCutOff,
        unsigned int number);

    void AddPointLight(const std::string& lightName,
        glm::vec3 position,
        float constant,
        float linear,
        float quadratic,
        glm::vec3 ambient,
        glm::vec3 diffuse,
        glm::vec3 specular,
        unsigned int number);

    void AddCamera(const std::string& cameraName, const std::shared_ptr<Camera>& camera);
    void LoadModelToScene(const std::string& fullPath, const std::string& modelName);
    void PrepareScene();
    void DrawObjects(const std::unordered_map<std::string, Shader>& shaders) const;
    void UpdateSelectedCamera();
    void UpdateChosenObject();
    glm::mat4 GetViewMatrix();

private:
    // Path to dir from directory where .exe is
    std::string modelsDir = "../models";
};

#endif  // SCENE_H
