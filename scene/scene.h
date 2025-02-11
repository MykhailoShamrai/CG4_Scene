#ifndef SCENE_H
#define SCENE_H
#include <map>
#include <memory>
#include <string>
#include <unordered_map>

#include "../camera/camera.h"
#include "../lights/bindedLight.h"
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

    // Map of lights on scene
    std::pmr::unordered_map<std::string, std::shared_ptr<BindedLight>> Lights;
    std::shared_ptr<BindedLight> CurrentLight;
    std::string CurrentLightName;

    // Global Direction light
    DirLight DirectionalLight = DirLight
    {
        glm::vec3(0.0f, -1.0f, 0.0f),
        glm::vec3(0.2f, 0.2f, 0.2f),
        glm::vec3(0.5f, 0.5f, 0.5f),
        glm::vec3(1.0f, 1.0f, 1.0f)
    };

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
