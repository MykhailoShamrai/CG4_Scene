#ifndef WINDOW_H
#define WINDOW_H
#include <stdexcept>
#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <iostream>
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
};

#endif //WINDOW_H
