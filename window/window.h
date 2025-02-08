#ifndef WINDOW_H
#define WINDOW_H
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>


#include "../scene/scene.h"
class Window
{
public:
    Window(const unsigned short &width, const unsigned short &height, const std::string &title = "Scene");
    void GameLoop();
    ~Window();
private:
    unsigned short width, height;
    GLFWwindow* window;
    std::string title;
    Scene main_scene = Scene();
};

#endif //WINDOW_H
