#include "scene.h"

#include <iostream>
#include <ostream>
#include <utility>

#include "../animator/UFOAnimator.h"
#include "../lights/pointLight.h"
#include "../lights/spotLight.h"
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

void Scene::AddSpotLight(
    const std::string& lightName, const glm::vec3& position, const glm::vec3& direction,
    const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular,
    const float& cutOff, const float& outerCutOff,unsigned int number)
{
    Lights.insert(
        {lightName, std::make_shared<SpotLight>(
                        position, direction, ambient, diffuse, specular, cutOff, outerCutOff, number
                    )}
    );
}
void Scene::AddPointLight(
    const std::string& lightName, glm::vec3 position, float constant, float linear, float quadratic,
    glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, unsigned int number
)
{
    Lights.insert(
        {lightName, std::make_shared<PointLight>(
                        position, constant, linear, quadratic, ambient, diffuse, specular, number
                    )}
    );
}

void Scene::AddCamera(const std::string& cameraName, const std::shared_ptr<Camera>& camera)
{
    std::cout << "Adding camera to scene" << std::endl;

    auto iteratorPair = Cameras.insert({cameraName, camera});
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
    auto basePtr = std::make_shared<AnimatorBase>();
    auto UfoAnimatorPointer = std::make_shared<UFOAnimator>();
    basePtr = std::static_pointer_cast<AnimatorBase>(UfoAnimatorPointer);

    AddCamera("FirstPersonFreeCamera", std::make_shared<Camera>(
            glm::vec3(0.0f, 30.0f, -30.0f), glm::vec3(0.0f, 0.1f, 0.0f),
            -90.0f, 0.0f
        ));
    auto mainCamera = Cameras.at("FirstPersonFreeCamera");
    mainCamera->SetMovable(true);
    mainCamera->UpdateCameraTarget(glm::vec3(0.0f, 0.0f, 0.0f));

    AddSpotLight(
        "spotLight0", glm::vec3(0.03f, 7.91f, 0.f), glm::vec3(0.f, 0.f, -1.f),
        glm::vec3(0.1f, 0.3f, 0.1f), glm::vec3(0.0f, 1.0f, 0.0f),
        glm::vec3(0.2f, 1.0f, 0.2f), 12.5, 15.0,
        0
    );
    auto sp1 = Lights.at("spotLight0");
    CurrentLightName = "spotLight0";
    CurrentLight = std::dynamic_pointer_cast<SpotLight>(sp1);

    AddPointLight(
        "pointLight0", glm::vec3(0.f, 0.f, 0.f), 1.0f, 0.09f, 0.032f, glm::vec3(0.5, 0.15f, 0.05f),
        glm::vec3(1.0f, 0.5f, 0.2f), glm::vec3(0.8f, 0.4f, 0.1f), 0
    );
    auto pl1 = Lights.at("pointLight0");

    stbi_set_flip_vertically_on_load(true);
    LoadModelToScene("../models/backpack/backpack.obj", "backpack1");
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
    ufo->SetDefaultSpecularAndShininess(glm::vec3(0.7f, 0.7f, 0.7f), 90.0f);
    ufo->SetSpecularAndShininess(glm::vec3(0.7f, 0.7f, 0.7f), 90.0f);
    ufo->SetXPosition(0.03f);
    ufo->SetYPosition(7.91f);
    ufo->SetXRotation(-90.0f);
    ufo->SetScale(2.8f);
    ufo->SetAnimated(basePtr);
    ufo->SetOldPos(ufo->GetWorldPosition());

    AddCamera(
        "ThirdPersonCamera",
        std::make_shared<Camera>(
            glm::vec3(25.0f, 15.0f, 15.0f), glm::vec3(0.0f, 0.1f, 0.0f),
            -90.0f, 0.0f
        )
    );

    auto thirdPersonCamera = Cameras.at("ThirdPersonCamera");
    thirdPersonCamera->BindObject = ufo;
    thirdPersonCamera->SetThirdPerson(true);

    AddCamera("WatchUfoCamera",std::make_shared<Camera>(
            glm::vec3(-40.0f, 35.0f, 0.0f), glm::vec3(0.0f, 0.1f, 0.0f),
            -90.0f, 0.0f));

    auto WatchUfoCamera = Cameras.at("WatchUfoCamera");
    WatchUfoCamera->BindObject = ufo;
    WatchUfoCamera->SetIsFindingObject(true);

    sp1->BindToObject(ufo);
    sp1->ChangePositionToObject();

    LoadModelToScene("../models/grass_field/scene.gltf", "grass1");
    auto grass = Drawables.at("grass1");
    grass->SetYPosition(-9.1f);
    grass->SetScale(0.1f);
    grass->SetDefaultSpecularAndShininess(glm::vec3(0.05f, 0.05f, 0.05f), 20.0f);
    grass->SetSpecularAndShininess(glm::vec3(0.05f, 0.05f, 0.05f), 20.0f);

    stbi_set_flip_vertically_on_load(false);
    LoadModelToScene("../models/cow/scene.gltf", "cow1");
    auto cow = Drawables.at("cow1");
    cow->SetXPosition(0.03f);
    cow->SetYPosition(-7.13f);
    cow->SetDefaultSpecularAndShininess(glm::vec3(0.1f, 0.1f, 0.1f), 20.0f);
    cow->SetSpecularAndShininess(glm::vec3(0.1f, 0.1f, 0.1f), 20.0f);

    LoadModelToScene("../models/pine_tree/scene.gltf", "pine_tree1");
    auto pine = Drawables.at("pine_tree1");
    pine->SetXPosition(15.3f);
    pine->SetYPosition(-7.1f);
    pine->SetXRotation(-90.0f);
    pine->SetDefaultSpecularAndShininess(glm::vec3(0.1f, 0.15f, 0.1f), 30.0f);
    pine->SetSpecularAndShininess(glm::vec3(0.1f, 0.15f, 0.1f), 30.0f);

    LoadModelToScene("../models/pine_tree/scene.gltf", "pine_tree2");
    pine = Drawables.at("pine_tree2");
    pine->SetXPosition(15.3f);
    pine->SetZPosition(11.4f);
    pine->SetYPosition(-7.1f);
    pine->SetXRotation(-90.0f);
    pine->SetDefaultSpecularAndShininess(glm::vec3(0.1f, 0.15f, 0.1f), 30.0f);
    pine->SetSpecularAndShininess(glm::vec3(0.1f, 0.15f, 0.1f), 30.0f);

    LoadModelToScene("../models/pine_tree/scene.gltf", "pine_tree3");
    pine = Drawables.at("pine_tree3");
    pine->SetXPosition(-15.3f);
    pine->SetYPosition(-7.0f);
    pine->SetXRotation(-90.0f);
    pine->SetScale(0.88f);
    pine->SetYRotation(-9.0f);
    pine->SetDefaultSpecularAndShininess(glm::vec3(0.1f, 0.15f, 0.1f), 30.0f);
    pine->SetSpecularAndShininess(glm::vec3(0.1f, 0.15f, 0.1f), 30.0f);

    LoadModelToScene("../models/pine_tree/scene.gltf", "pine_tree4");
    pine = Drawables.at("pine_tree4");
    pine->SetXPosition(4.3f);
    pine->SetYPosition(-7.2f);
    pine->SetXRotation(-90.0f);
    // pine->SetYRotation(-100.0f);
    pine->SetScale(0.94f);
    pine->SetDefaultSpecularAndShininess(glm::vec3(0.1f, 0.15f, 0.1f), 30.0f);
    pine->SetSpecularAndShininess(glm::vec3(0.1f, 0.15f, 0.1f), 30.0f);

    LoadModelToScene("../models/pine_tree/scene.gltf", "pine_tree5");
    pine = Drawables.at("pine_tree5");
    pine->SetXPosition(3.0f);
    pine->SetZPosition(11.0f);
    pine->SetYPosition(-7.1f);
    pine->SetXRotation(-90.0f);
    pine->SetDefaultSpecularAndShininess(glm::vec3(0.1f, 0.15f, 0.1f), 30.0f);
    pine->SetSpecularAndShininess(glm::vec3(0.1f, 0.15f, 0.1f), 30.0f);

    LoadModelToScene("../models/pine_tree/scene.gltf", "pine_tree6");
    pine = Drawables.at("pine_tree6");
    pine->SetXPosition(5.3f);
    pine->SetXPosition(-7.1f);
    pine->SetYPosition(-6.6f);
    pine->SetXRotation(-90.0f);
    pine->SetDefaultSpecularAndShininess(glm::vec3(0.1f, 0.15f, 0.1f), 30.0f);
    pine->SetSpecularAndShininess(glm::vec3(0.1f, 0.15f, 0.1f), 30.0f);

    LoadModelToScene("../models/pine_tree/scene.gltf", "pine_tree7");
    pine = Drawables.at("pine_tree7");
    pine->SetXPosition(-5.29f);
    pine->SetZPosition(12.19f);
    pine->SetYPosition(-7.1f);
    pine->SetXRotation(-90.0f);
    pine->SetDefaultSpecularAndShininess(glm::vec3(0.1f, 0.15f, 0.1f), 30.0f);
    pine->SetSpecularAndShininess(glm::vec3(0.1f, 0.15f, 0.1f), 30.0f);

    LoadModelToScene("../models/bonfire__fire/scene.gltf", "bonfire1");
    auto fire = Drawables.at("bonfire1");
    fire->SetXPosition(3.97f);
    fire->SetYPosition(-7.34f);
    fire->SetZPosition(-2.93f);
    fire->SetScale(0.5f);

    pl1->BindToObject(fire);
    pl1->ChangePositionToObject();


    AddPointLight(
    "pointLight1", glm::vec3(0.f, 0.f, 0.f), 1.0f, 0.09f, 0.032f, glm::vec3(1.0, 1.0f, 1.0f),
    glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), 1
    );

    auto pl2 = Lights.at("pointLight1");
    LoadModelToScene("../models/sphere/scene.gltf", "sphere1");
    auto sphere = Drawables.at("sphere1");
    sphere->SetXPosition(4.0f);
    sphere->SetYPosition(-6.5f);
    sphere->SetZPosition(6.0f);
    sphere->SetScale(0.1f);
    pl2->BindToObject(sphere);
    pl2->ChangePositionToObject();

    LoadModelToScene("../models/sphere/scene.gltf", "sphereChrome");
    sphere = Drawables.at("sphereChrome");
    sphere->SetCustomMaterial(glm::vec3(0.25f, 0.25f, 0.25f), glm::vec3(0.4f, 0.4f, 0.4f),
        glm::vec3(0.774597f, 0.774597f, 0.774597f), 0.6 * 128);
    sphere->SetDefaultMaterial(glm::vec3(0.25f, 0.25f, 0.25f), glm::vec3(0.4f, 0.4f, 0.4f),
    glm::vec3(0.774597f, 0.774597f, 0.774597f), 0.6 * 128);
}

void Scene::DrawObjects(const std::unordered_map<std::string, Shader>& shaders) const
{
    for (const auto& drawable : Drawables)
    {
        drawable.second->Draw(shaders);
    }
}

std::vector<PredefinedMaterial> Drawable::PredefinedMaterials =  {
    {"emerald", glm::vec3(0.0215f, 0.1745f, 0.0215f),
                glm::vec3(0.07568f, 0.61424f, 0.07568f),
                glm::vec3(0.633f, 0.727811f, 0.633f),
                128 * 0.6f},
    {"jade", glm::vec3(0.135f, 0.2225f, 0.1575f),
            glm::vec3(0.54f, 0.89f, 0.63f),
            glm::vec3(0.316228f, 0.316228f, 0.316228f),
            128 * 0.1f},
    {"obsidian", glm::vec3(0.05375f, 0.05f, 0.06625f),
            glm::vec3(0.18275f, 0.17f, 0.22525f),
            glm::vec3(0.332741f, 0.328634f, 0.346435f),
            128 * 0.3f},
    {"pearl", glm::vec3(0.25f, 0.20725f, 0.20725f),
            glm::vec3(1.0f, 0.829f, 0.829f),
            glm::vec3(0.296648f, 0.296648f, 0.296648f),
            128 * 0.088f},
    {"ruby", glm::vec3(0.1745f, 0.01175f, 0.01175f),
            glm::vec3(0.61424f, 0.04136f, 0.04136f),
            glm::vec3(0.727811f, 0.626959f, 0.626959f),
            128 * 0.6f},
    {"gold", glm::vec3(0.24725f, 0.1995f, 0.0745f),
            glm::vec3(0.75164f, 0.60648f, 0.22648f),
            glm::vec3(0.628281f, 0.555802f, 0.366065f),
            128 * 0.4f},
    {"silver", glm::vec3(0.19225f, 0.19225f, 0.19225f),
            glm::vec3(0.50754f, 0.50754f, 0.50754f),
            glm::vec3(0.508273f, 0.508273f, 0.508273f),
            128 * 0.4f}
};

void Scene::UpdateSelectedCamera() { CurrentCamera = Cameras.at(CurrentCameraName); }
void Scene::UpdateChosenObject() { ChosenObject = Drawables.at(ChosenObjectName); }
