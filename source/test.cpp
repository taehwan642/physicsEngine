#include <iostream>
#include <Vector.h>
#include <glfw/glfw3.h>

int main()
{
    if (!glfwInit()) return -1;
    std::cout << "GLFW lib Init" << std::endl; 
    return 0;
}
