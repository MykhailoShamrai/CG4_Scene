#ifndef SCENE_H
#define SCENE_H
#include <map>
#include <memory>
#include <string>

#include "drawable.h"

class Scene
{
public:
    // Map of positions in world space
    std::pmr::map<std::string, std::unique_ptr<Drawable>> Drawables;
    // I assume that
    void LoadModelToScene(const std::string& fullPath, std::string modelName);
    void PrepareScene();
    void DrawObjects(const Shader& shader) const;
private:
    // Path to dir from directory where .exe is
    std::string modelsDir = "../models";
};

#endif //SCENE_H
