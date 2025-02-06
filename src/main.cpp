#include <iostream>
#include <ostream>

#include "../window/window.h"

#define WIDTH  1600
#define HEIGHT 900

int main(int argc, char** argv)
{
    try
    {
        Window mainWindow(WIDTH, HEIGHT, "Scene Demo");
        mainWindow.GameLoop();
        return 0;
    }
    catch (std::runtime_error& ex)
    {
        std::cout << ex.what() << std::endl;
        return -1;
    }
    return 0;
}
