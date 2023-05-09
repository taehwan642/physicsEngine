#include "Engine.hpp"

#include <iostream>

void InputCallback(GLFWwindow *window, int key, int scancode, int action,
                   int mods) {
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    glfwSetWindowShouldClose(window, GLFW_TRUE);
}

void FramebufferSizeCallback(GLFWwindow *window, int width, int height) {
  glViewport(0, 0, width, height);
}

void ErrorCallback(int error_code, const char *err_str) {
  std::cout << "GLFW Error: " << err_str << std::endl;
}

void Engine::Engine::AddScene(std::shared_ptr<Scene> scene) {
  scenes.push_back(scene);
}

void Engine::Engine::EnterScene(int sceneIndex) {
  if (currentScene != nullptr) currentScene->Exit();
  currentScene = scenes[sceneIndex];
  currentScene->Initialize();
}

bool Engine::Engine::Initialize() {
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
    return false;
  }

  // Close the window as soon as the Escape key has been pressed
  glfwSetKeyCallback(window, InputCallback);
  // Makes the window context current
  glfwMakeContextCurrent(window);
#ifdef _WIN64
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "could not load GLAD" << std::endl;
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

  ui = std::make_shared<UI>();
  if (ui->Initialize(this) == false) return false;

  return true;
}

void Engine::Engine::Update() {
  ui->Update();
  currentScene->Update();
}

void Engine::Engine::Render() {
  ui->Render();
  currentScene->Render();
}

void Engine::Engine::Exit() {
  ui->Exit();
  currentScene->Exit();
  glfwTerminate();
}

bool Engine::Engine::NeedsToCloseWindow() {
  return glfwWindowShouldClose(window);
}

GLFWwindow *Engine::Engine::GetWindow() { return window; }

size_t Engine::Engine::GetWidth() { return width; }

size_t Engine::Engine::GetHeight() { return height; }
