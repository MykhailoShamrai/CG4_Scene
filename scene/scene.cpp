#include "scene.h"

#include <iostream>
#include <ostream>
#include <utility>

#include "glm/gtx/integer.hpp"
#include "model.h"
#include "stb_image.h"

void Scene::LoadModelToScene(const std::string& fullPath, const std::string& modelName)
{
    try
    {
        std::cout << "Loaded model: " << modelName << std::endl;
        std::shared_ptr<Model> model = std::make_shared<Model>(fullPath);
        Drawables.insert({modelName, model});
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
    CurrentCameraName = cameraName;
    UpdateSelectedCamera();
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

    AddCamera("secondCamera", std::make_shared<Camera>(glm::vec3(25.0f, 15.0f, 15.0f),
        glm::vec3(0.0f, 0.1f, 0.0f), -90.0f, 0.0f));
    Cameras.at("secondCamera")->updateCameraTarget(glm::vec3(0.0f, 0.0f, 0.0f));


    stbi_set_flip_vertically_on_load(true);
    LoadModelToScene("../models/backpack/backpack.obj", "backpack1");
    assert(Drawables.size() > 0);
    assert(Drawables.find("backpack1") != Drawables.end());
    auto dr = Drawables.at("backpack1");
    dr->SetZPosition(-4.2f);
    dr->SetYPosition(-7.020f);
    dr->SetXRotation(300.5f);
    dr->SetYRotation(-15.5);
    dr->SetZRotation(0.5f);
    dr->SetScale(0.2f);

    stbi_set_flip_vertically_on_load(false);
    LoadModelToScene("../models/ufo/scene.gltf", "ufo1");
    auto ufo = Drawables.at("ufo1");
    ufo->SetXPosition(0.03f);
    ufo->SetYPosition(10.0f);
    ufo->SetXRotation(-90.0f);
    ufo->SetScale(3.0f);

    LoadModelToScene("../models/grass_field/scene.gltf", "grass1");
    auto grass = Drawables.at("grass1");
    grass->SetYPosition(-9.1f);
    grass->SetScale(0.1f);

    stbi_set_flip_vertically_on_load(false);
    LoadModelToScene("../models/cow/scene.gltf", "cow1");
    auto cow = Drawables.at("cow1");
    cow->SetXPosition(0.03f);
    cow->SetYPosition(-7.13f);
}

void Scene::DrawObjects(const Shader& shader) const
{
    for (const auto& drawable : Drawables)
    {
        drawable.second->Draw(shader);
    }
}

void Scene::UpdateSelectedCamera() { CurrentCamera = Cameras.at(CurrentCameraName); }
void Scene::UpdateChosenObject() { ChosenObject = Drawables.at(ChosenObjectName); }
