#include "window.h"

#include <stdexcept>

#include "glm/gtc/matrix_transform.hpp"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

Window::Window(const unsigned short &width, const unsigned short &height, const std::string &title):
    width(width), height(height), title(title)
{
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
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init();
}



void Window::GameLoop()
{
    // Prepare start scene
    main_scene.PrepareScene();
    // Main shaders
    Shader mainShader = Shader("../shaders/vertexDefaultShader.vert",
        "../shaders/fragmentDefaultShader.frag");

    // Projection matrix. It is passed into shader in window functions, because width and height
    // depend on window configuration
    glm::mat4 proj = glm::perspective(glm::radians(45.0f), static_cast<float>(width)
        / static_cast<float>(height), 0.1f, 100.0f);

    glClearColor(0.1f, 0.2f, 0.0f, 1.0f);
    while (!glfwWindowShouldClose(window))
    {
        // Imgui window
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::Text("Options");
        float fps = ImGui::GetIO().Framerate;
        ImGui::Text("FPS: %.1f", fps);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        mainShader.Use();
        mainShader.SetMat4("view", glm::mat4(1.0f));
        // Projection matrix is passed here.
        mainShader.SetMat4("projection", proj);
        mainShader.SetMat3("normalMatrix", glm::mat3(1.0f));
        main_scene.DrawObjects(mainShader);

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