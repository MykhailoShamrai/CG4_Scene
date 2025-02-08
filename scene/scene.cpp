#include "scene.h"

#include <iostream>
#include <ostream>
#include <utility>

#include "model.h"

void Scene::LoadModelToScene(const std::string& fullPath, std::string modelName)
{
    try
    {
        std::cout << "Loaded model: " << modelName << std::endl;
        Drawables.insert({modelName, std::make_unique<Model>(fullPath)});
        return;
    }
    catch (std::exception e)
    {
        std::cout << "Error while loading model: " << e.what() << std::endl;
        return;
    }
}
void Scene::PrepareScene()
{
    LoadModelToScene("../models/backpack/backpack.obj", "backpack");
}

void Scene::DrawObjects(const Shader &shader) const
{
    for (const auto& drawable : Drawables) {
        drawable.second->Draw(shader);
    }
}
