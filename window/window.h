#ifndef WINDOW_H
#define WINDOW_H
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <vector>

#include "../scene/scene.h"
class Window
{
public:
    Window(
        const unsigned short& width, const unsigned short& height,
        const std::string& title = "Scene"
    );
    void GameLoop();
    ~Window();
    bool FPSmode    = false;
    bool FirstMouse = true;
    bool Day        = true;
    bool Fog        = false;
    bool BlinnPhong = false;

    float FogMaxDist  = 100.0f;
    float FogMinDist   = 0.1;
    glm::vec3 DayColor = glm::vec3(0.53f, 0.81f, 0.98f);
    glm::vec3 NightColor = glm::vec3(0.02f, 0.02f, 0.1f);
    glm::vec3 FogColor = glm::vec3(0.4, 0.4, 0.4);

private:
    unsigned short width, height;
    GLFWwindow* window;
    std::string title;
    // Time between frames
    float deltaTime = 0.0f;
    float lastFrame = 0.0f;
    float lastX;
    float lastY;

    std::unordered_map<std::string, Shader> shaders = std::unordered_map<std::string, Shader>();
    // Main scene that should be visible
    Scene mainScene = Scene();

    void createShaders();
    void processMovement(GLFWwindow* window);
    void mouse_callback(GLFWwindow* window, double xpos, double ypos);
    void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
    void changeDayNightMode(bool mode);
    void changeFogMode(bool mode);
    void changeBlinnPhongMode(bool mode);
    void renderGuiCameras();
    void renderGuiObjects();
};

#endif //WINDOW_H
