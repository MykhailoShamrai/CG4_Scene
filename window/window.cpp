#include "window.h"

#include <stdexcept>

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

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init();

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
}

void Window::GameLoop()
{
    // Prepare start scene
    mainScene.PrepareScene();
    // Main shaders
    Shader mainShader = Shader("../shaders/vertexDefaultShader.vert", "../shaders/fragmentDefaultShader.frag");

    // Projection matrix. It is passed into shader in window functions, because width and height
    // depend on window configuration
    glm::mat4 proj =
        glm::perspective(glm::radians(45.0f), static_cast<float>(width) / static_cast<float>(height), 0.1f, 100.0f);

    glClearColor(0.1f, 0.2f, 0.0f, 1.0f);
    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime          = currentFrame - lastFrame;
        lastFrame          = currentFrame;
        // Imgui window
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::Text("Options");
        float fps = ImGui::GetIO().Framerate;
        ImGui::Text("FPS: %.1f", fps);

        // Input for camera
        processMovement(window);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        mainShader.Use();
        glm::mat4 view = glm::mat4(1.0f);
        view           = mainScene.GetViewMatrix();
        mainShader.SetMat4("view", view);
        // Projection matrix is passed here.
        mainShader.SetMat4("projection", proj);
        mainScene.DrawObjects(mainShader);

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
