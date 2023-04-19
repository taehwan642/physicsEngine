#include "Engine.hpp"

#include <iostream>

void InputCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
}

void FramebufferSizeCallback(GLFWwindow *window, int width, int height)
{
}

void ErrorCallback(int error_code, const char *err_str)
{
    
}

bool Engine::Engine::Initialize()
{
  glfwSetErrorCallback(ErrorCallback);
  // Initialize the lib
  if (!glfwInit()) {
    std::cout << "could not start GLFW3" << std::endl;
    return false;
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  window = glfwCreateWindow(width, height, window_name.data(), NULL, NULL);
  if (window == nullptr) {
    std::cout << "window creation failed" << std::endl;
    glfwTerminate();
    return false;
  }

  // Close the window as soon as the Escape key has been pressed
  glfwSetKeyCallback(window, InputCallback);
  // Makes the window context current
  glfwMakeContextCurrent(window);
#ifdef _WIN64
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "could not load GLAD" << std::endl;
    glfwTerminate();
    return false;
  }
#endif
  // Enable the viewport
  // cannot use gl function before initilize (defined region code)
  glfwGetFramebufferSize(window, &framebufferWidth, &framebufferHeight);
  glViewport(0, 0, framebufferWidth, framebufferHeight);

  glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback);

  // Note: Once you have a current OpenGL context, you can use OpenGL normally
  // get version info
  const GLubyte *renderer = glGetString(GL_RENDERER);
  const GLubyte *version = glGetString(GL_VERSION);
  std::cout << "Renderer: " << renderer << std::endl;
  std::cout << "OpenGL version supported: " << version << std::endl;

  return true;
}

void Engine::Engine::Update()
{
}

void Engine::Engine::Render()
{
}

void Engine::Engine::Exit()
{
}

bool Engine::Engine::NeedsToCloseWindow()
{
  return glfwWindowShouldClose(window);
}

GLFWwindow* Engine::Engine::GetWindow()
{
  return window;
}