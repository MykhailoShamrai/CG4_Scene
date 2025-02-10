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
    ufo->SetSpecularAndShininess(glm::vec3(0.7f, 0.7f, 0.7f), 90.0f);
    ufo->SetXPosition(0.03f);
    ufo->SetYPosition(7.91f);
    ufo->SetXRotation(-90.0f);
    ufo->SetScale(2.8f);

    LoadModelToScene("../models/grass_field/scene.gltf", "grass1");
    auto grass = Drawables.at("grass1");
    grass->SetYPosition(-9.1f);
    grass->SetScale(0.1f);
    grass->SetSpecularAndShininess(glm::vec3(0.05f, 0.05f, 0.05f), 20.0f);

    stbi_set_flip_vertically_on_load(false);
    LoadModelToScene("../models/cow/scene.gltf", "cow1");
    auto cow = Drawables.at("cow1");
    cow->SetXPosition(0.03f);
    cow->SetYPosition(-7.13f);
    cow->SetSpecularAndShininess(glm::vec3(0.1f, 0.1f, 0.1f), 20.0f);

    LoadModelToScene("../models/pine_tree/scene.gltf", "pine_tree1");
    auto pine = Drawables.at("pine_tree1");
    pine->SetXPosition(15.3f);
    pine->SetYPosition(-7.1f);
    pine->SetXRotation(-90.0f);
    pine->SetSpecularAndShininess(glm::vec3(0.1f, 0.15f, 0.1f), 30.0f);

    LoadModelToScene("../models/pine_tree/scene.gltf", "pine_tree2");
    pine = Drawables.at("pine_tree2");
    pine->SetXPosition(15.3f);
    pine->SetZPosition(11.4f);
    pine->SetYPosition(-7.1f);
    pine->SetXRotation(-90.0f);
    pine->SetSpecularAndShininess(glm::vec3(0.1f, 0.15f, 0.1f), 30.0f);

    LoadModelToScene("../models/pine_tree/scene.gltf", "pine_tree3");
    pine = Drawables.at("pine_tree3");
    pine->SetXPosition(-15.3f);
    pine->SetYPosition(-7.0f);
    pine->SetXRotation(-90.0f);
    pine->SetScale(0.88f);
    pine->SetYRotation(-9.0f);
    pine->SetSpecularAndShininess(glm::vec3(0.1f, 0.15f, 0.1f), 30.0f);

    LoadModelToScene("../models/pine_tree/scene.gltf", "pine_tree4");
    pine = Drawables.at("pine_tree4");
    pine->SetXPosition(4.3f);
    pine->SetYPosition(-7.2f);
    pine->SetXRotation(-90.0f);
    //pine->SetYRotation(-100.0f);
    pine->SetScale(0.94f);
    pine->SetSpecularAndShininess(glm::vec3(0.1f, 0.15f, 0.1f), 30.0f);

    LoadModelToScene("../models/pine_tree/scene.gltf", "pine_tree5");
    pine = Drawables.at("pine_tree5");
    pine->SetXPosition(3.0f);
    pine->SetZPosition(11.0f);
    pine->SetYPosition(-7.1f);
    pine->SetXRotation(-90.0f);
    pine->SetSpecularAndShininess(glm::vec3(0.1f, 0.15f, 0.1f), 30.0f);

    LoadModelToScene("../models/pine_tree/scene.gltf", "pine_tree6");
    pine = Drawables.at("pine_tree6");
    pine->SetXPosition(5.3f);
    pine->SetXPosition(-7.1f);
    pine->SetYPosition(-6.6f);
    pine->SetXRotation(-90.0f);
    pine->SetSpecularAndShininess(glm::vec3(0.1f, 0.15f, 0.1f), 30.0f);

    LoadModelToScene("../models/pine_tree/scene.gltf", "pine_tree7");
    pine = Drawables.at("pine_tree7");
    pine->SetXPosition(-5.29f);
    pine->SetZPosition(12.19f);
    pine->SetYPosition(-7.1f);
    pine->SetXRotation(-90.0f);
    pine->SetSpecularAndShininess(glm::vec3(0.1f, 0.15f, 0.1f), 30.0f);

    LoadModelToScene("../models/bonfire__fire/scene.gltf", "bonfire1");
    auto fire = Drawables.at("bonfire1");
    fire->SetXPosition(3.97f);
    fire->SetYPosition(-7.34f);
    fire->SetZPosition(-2.93f);
    fire->SetScale(0.5f);

}

void Scene::DrawObjects(const std::unordered_map<std::string, Shader>& shaders) const
{
    for (const auto& drawable : Drawables)
    {
        drawable.second->Draw(shaders);
    }
}

void Scene::UpdateSelectedCamera() { CurrentCamera = Cameras.at(CurrentCameraName); }
void Scene::UpdateChosenObject() { ChosenObject = Drawables.at(ChosenObjectName); }
