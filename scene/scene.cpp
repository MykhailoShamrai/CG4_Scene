#include "scene.h"

#include <iostream>
#include <ostream>
#include <utility>

#include "model.h"

void Scene::LoadModelToScene(const std::string& fullPath, const std::string& modelName)
{
    try
    {
        std::cout << "Loaded model: " << modelName << std::endl;
        Drawables.insert({modelName, std::make_shared<Model>(fullPath)});
        return;
    }
    catch (std::exception e)
    {
        std::cout << "Error while loading model: " << e.what() << std::endl;
        return;
    }
}

void Scene::AddCamera(const std::string& cameraName, const std::shared_ptr<Camera>& camera)
{
    std::cout << "Adding camera to scene" << std::endl;

    auto iteratorPair =  Cameras.insert({cameraName, camera});
    // Make inserted or already existing camera ass current
    CurrentCamera = iteratorPair.first->second;
}

glm::mat4 Scene::GetViewMatrix()
{
    if (CurrentCamera == nullptr)
        throw std::runtime_error("No camera found");

    return CurrentCamera->GetView();
}

void Scene::PrepareScene()
{
    AddCamera("mainCamera", std::make_shared<Camera>());
    Cameras.at("mainCamera")->SetMovable(true);
    LoadModelToScene("../models/backpack/backpack.obj", "backpack");
    assert(Drawables.size() > 0);
    assert(Drawables.find("backpack") != Drawables.end());
    auto dr = Drawables.at("backpack");
    dr->SetZPosition(-15.0f);
    dr->SetZRotation(35.0f);
}

void Scene::DrawObjects(const Shader& shader) const
{
    for (const auto& drawable : Drawables)
    {
        drawable.second->Draw(shader);
    }
}
