#include "window.h"

#include <algorithm>
#include <functional>
#include <stdexcept>
#include <vector>

#include "glm/gtc/matrix_transform.hpp"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

Window::Window(const unsigned short &width, const unsigned short &height, const std::string &title)
    : width(width), height(height), title(title)
{
    lastX = static_cast<float>(width) / 2.0f;
    lastY = static_cast<float>(height) / 2.0f;
    // Initialization of glfw stuff
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
    if (window == nullptr)
    {
        glfwTerminate();
        throw std::runtime_error("Failed to create GLFW window");
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
    {
        throw std::runtime_error("Failed to initialize GLAD");
    }

    glEnable(GL_DEPTH_TEST);
    // ImGui initialization
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;


    glfwSetWindowUserPointer(window, this);
    auto mouseCallback = [](GLFWwindow *w, double xpos, double ypos)
    {
        static_cast<Window *>(glfwGetWindowUserPointer(w))->mouse_callback(w, xpos, ypos);
    };

    auto keyCallback = [](GLFWwindow *w, int key, int scancode, int action, int mods)
    {
        static_cast<Window *>(glfwGetWindowUserPointer(w))->key_callback(w, key, scancode, action, mods);
    };

    glfwSetKeyCallback(window, keyCallback);
    glfwSetCursorPosCallback(window, mouseCallback);

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init();
}

void Window::GameLoop()
{
    // Prepare start scene
    mainScene.PrepareScene();

    // Projection matrix. It is passed into shader in window functions, because width and height
    // depend on window configuration
    glm::mat4 proj =
        glm::perspective(glm::radians(45.0f), static_cast<float>(width) / static_cast<float>(height), 0.1f, 100.0f);

    createShaders();
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime          = currentFrame - lastFrame;
        lastFrame          = currentFrame;


        // ------------- IMGUI PART OF RENDERING ---------------- //
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        renderGuiCameras();
        renderGuiObjects();

        // Input for camera
        processMovement(window);
        for (const auto &drawable: mainScene.Drawables)
        {
            drawable.second->Animate(deltaTime);
        }
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        for (const auto &light: mainScene.Lights)
        {
            light.second->ChangePositionToObject();
        }

        for (const auto &shader : shaders)
        {
            for (const auto &light: mainScene.Lights)
            {
                light.second->UseInShader(shader.second);
            }
            shader.second.Use();
            shader.second.SetVec3("viewerPos", mainScene.CurrentCamera->GetCameraPosition());
            shader.second.SetVec3("dirLight.direction", mainScene.DirectionalLight.Direction);
            shader.second.SetVec3("dirLight.ambient", mainScene.DirectionalLight.Ambient);
            shader.second.SetVec3("dirLight.diffuse", mainScene.DirectionalLight.Diffuse);
            shader.second.SetVec3("dirLight.specular", mainScene.DirectionalLight.Specular);

            shader.second.SetMat4("projection", proj);
            glm::mat4 view = glm::mat4(1.0f);
            view           = mainScene.GetViewMatrix();
            shader.second.SetMat4("view", view);
            // Projection matrix is passed here.
            shader.second.SetMat4("projection", proj);
        }


        mainScene.DrawObjects(shaders);

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

Window::~Window()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();
}

void Window::createShaders()
{
    shaders.insert( {"programModelTextures",
        Shader("../shaders/vertexDefaultShader.vert",
            "../shaders/fragmentModelWithSpecular.frag")});

    shaders.insert( {"programModelWithoutSpecular",
        Shader("../shaders/vertexDefaultShader.vert",
            "../shaders/fragmentModelWithoutSpecular.frag")});

    shaders.insert({"programMaterials",
        Shader("../shaders/vertexDefaultShader.vert",
        "../shaders/fragmentMaterial.frag")});
}
void Window::processMovement(GLFWwindow *window)
{
    // TODO: Make that no all cameras can do it
    if (mainScene.CurrentCamera->GetMovable())
    {
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            mainScene.CurrentCamera->ProcessKeyboard(FORWARD, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            mainScene.CurrentCamera->ProcessKeyboard(BACKWARD, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            mainScene.CurrentCamera->ProcessKeyboard(LEFT, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            mainScene.CurrentCamera->ProcessKeyboard(RIGHT, deltaTime);
    }
}

void Window::mouse_callback(GLFWwindow *window, double xpos, double ypos)
{
    if (FPSmode && mainScene.CurrentCamera->GetMovable())
    {
        if (FirstMouse)
        {
            lastX      = xpos;
            lastY      = ypos;
            FirstMouse = false;
        }

        float xoffset = xpos - lastX;
        float yoffset = lastY - ypos;
        lastX = xpos;
        lastY = ypos;

        float sensitivity = 0.8f;
        xoffset *= sensitivity;
        yoffset *= sensitivity;

        mainScene.CurrentCamera->ProcessMouseMovement(xoffset, yoffset);
    }
}
void Window::key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_F && action == GLFW_PRESS)
    {
        if (FPSmode)
        {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            FPSmode = false;
        }
        else
        {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            FirstMouse = true;
            FPSmode    = true;
        }
    }
}

void Window::renderGuiCameras()
{
    ImGui::Begin("Cameras");
    float fps = ImGui::GetIO().Framerate;
    ImGui::Text("FPS: %.1f", fps);

    // Part with list box
    std::vector<const char *> keys;
    for (const auto &kv : mainScene.Cameras)
    {
        keys.push_back(kv.first.c_str());
    }

    int currentIndex = -1;
    if (!mainScene.CurrentCameraName.empty())
    {
        auto iterator = std::ranges::find(keys, mainScene.CurrentCameraName.c_str());
        if (iterator != keys.end())
        {
            currentIndex = std::distance(keys.begin(), iterator);
        }
    }

    if (ImGui::ListBox("Choose camera", &currentIndex, keys.data(), static_cast<int>(keys.size())))
    {
        if (currentIndex >= 0 && currentIndex < static_cast<int>(keys.size()))
        {
            mainScene.CurrentCameraName = keys[currentIndex];
            mainScene.UpdateSelectedCamera();
        }
    }
    ImGui::Text(mainScene.CurrentCameraName.c_str());

    ImGui::End();
}

void Window::renderGuiObjects()
{
    ImGui::Begin("Movable objects");

    std::vector<const char *> keys;
    for (const auto &kv : mainScene.Drawables)
    {
        keys.push_back(kv.first.c_str());
    }

    int currentIndex = -1;
    if (!mainScene.ChosenObjectName.empty())
    {
        auto iterator = std::ranges::find(keys, mainScene.ChosenObjectName.c_str());
        if (iterator != keys.end())
        {
            currentIndex = std::distance(keys.begin(), iterator);
        }
    }

    if (ImGui::ListBox("Choose object", &currentIndex, keys.data(), static_cast<int>(keys.size())))
    {
        if (currentIndex >= 0 && currentIndex < static_cast<int>(keys.size()))
        {
            mainScene.ChosenObjectName = keys[currentIndex];
            mainScene.UpdateChosenObject();
        }
    }
    ImGui::Text(mainScene.ChosenObjectName.c_str());

    ImGui::End();

    if (mainScene.ChosenObject)
    {

        ImGui::Begin("Object Properties");
        assert(mainScene.ChosenObject != nullptr);
        Drawable& chosenObject = *mainScene.ChosenObject;

        ImGui::Text("Selected object: %s", mainScene.ChosenObjectName.c_str());

        glm::vec3 position = chosenObject.GetWorldPosition();
        float pos[3] = { position.x, position.y, position.z };
        glm::vec3 rotation = chosenObject.GetRotation();
        float rot[3] = { rotation.x, rotation.y, rotation.z };
        float scale = chosenObject.GetScale();

        // Setting buttons... A little of spaghetti
# pragma region Position changing
        bool changedPosition = false;
        if (ImGui::Button("-X"))
        {
            pos[0] -= 0.01f;
            changedPosition = true;
        }
        ImGui::SameLine();
        if (ImGui::Button("+X"))
        {
            pos[0] += 0.01f;
            changedPosition = true;
        }
        ImGui::SameLine();
        if (ImGui::Button("-Y"))
        {
            pos[1] -= 0.01f;
            changedPosition = true;
        }
        ImGui::SameLine();
        if (ImGui::Button("+Y"))
        {
            pos[1] += 0.01f;
            changedPosition = true;
        }
        ImGui::SameLine();
        if (ImGui::Button("-Z"))
        {
            pos[2] -= 0.01f;
            changedPosition = true;
        }
        ImGui::SameLine();
        if (ImGui::Button("+Z"))
        {
            pos[2] += 0.01f;
            changedPosition = true;
        }
        if (changedPosition || ImGui::DragFloat3("Position", pos, 0.1f))
        {
            chosenObject.SetXPosition(pos[0]);
            chosenObject.SetYPosition(pos[1]);
            chosenObject.SetZPosition(pos[2]);
        }
# pragma endregion
# pragma region Rotation changing
        bool changedRotation = false;
        if (ImGui::Button("-Xangle"))
        {
            rot[0] -= 0.5f;
            changedRotation = true;
        }
        ImGui::SameLine();
        if (ImGui::Button("+Xangle"))
        {
            rot[0] += 0.5f;
            changedRotation = true;
        }

        if (ImGui::Button("-Yangle"))
        {
            rot[1] -= 0.5f;
            changedRotation = true;
        }
        ImGui::SameLine();
        if (ImGui::Button("+Yangle"))
        {
            rot[1] += 0.5f;
            changedRotation = true;
        }

        if (ImGui::Button("-Zangle"))
        {
            rot[2] -= 0.5f;
            changedRotation = true;
        }
        ImGui::SameLine();
        if (ImGui::Button("+Zangle"))
        {
            rot[2] += 0.5f;
            changedRotation = true;
        }

        if (changedRotation || ImGui::DragFloat3("Rotation", rot, 0.1f, 0.0f, 0.0f, "%.1f"))
        {
            chosenObject.SetXRotation(rot[0]);
            chosenObject.SetYRotation(rot[1]);
            chosenObject.SetZRotation(rot[2]);
        }
#pragma endregion
# pragma region Scale changing
        bool changedScale = false;
        if (ImGui::Button("+scale"))
        {
            scale += 0.1f;
            changedScale = true;
        }
        ImGui::SameLine();
        if (ImGui::Button("-scale"))
        {
            scale -= 0.1f;
            changedScale = true;
        }
        if (changedScale || ImGui::DragFloat("Scale", &scale, 0.1f, 0.0f, 0.0f, "%.1f"))
        {
            chosenObject.SetScale(scale);
        }
# pragma endregion
        ImGui::End();
    }

    // For Light
# pragma region Light changing
    if (mainScene.CurrentLight != nullptr)
    {
        ImGui::Begin("Change Light direction");
        glm::vec3& direction = mainScene.CurrentLight->Direction;
        if (ImGui::SliderFloat3("Direction", glm::value_ptr(direction), -1.0f, 1.0f))
            mainScene.CurrentLight->ChangeDirVector(direction);

        ImGui::End();
    }
# pragma endregion
}
