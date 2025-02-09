#ifndef SCENE_H
#define SCENE_H
#include <map>
#include <memory>
#include <string>
#include <unordered_set>

#include "../camera/camera.h"
#include "drawable.h"

class Scene
{
public:
    // Map of positions in world space
    std::pmr::map<std::string, std::shared_ptr<Drawable>> Drawables;
    // Set of cameras on scene
    std::pmr::map<std::string, std::shared_ptr<Camera>> Cameras;
    std::shared_ptr<Camera> CurrentCamera;
    void AddCamera(const std::string& cameraName, const std::shared_ptr<Camera>& camera);
    void LoadModelToScene(const std::string& fullPath, const std::string& modelName);
    void PrepareScene();
    void DrawObjects(const Shader& shader) const;
    glm::mat4 GetViewMatrix();

private:
    // Path to dir from directory where .exe is
    std::string modelsDir = "../models";
};

#endif  // SCENE_H
