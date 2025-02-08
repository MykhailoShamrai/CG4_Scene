#include "scene.h"

#include <iostream>
#include <ostream>

#include "model.h"

void Scene::LoadModelToScene(const std::string& fullPath, std::string modelName)
{
    try
    {
        Model model = Model(fullPath);
        std::cout << "Loaded model: " << modelName << std::endl;
        Drawables.insert({modelName, model});
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
