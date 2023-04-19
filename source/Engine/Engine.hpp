#pragma once

#include <memory>
#include <string>

#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
#define GLFW_INCLUDE_GLCOREARB
#include <GLFW/glfw3.h>
#endif
#ifdef _WIN64
#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#endif

namespace Engine {
class Engine {
 private:
  const size_t width = 640;
  const size_t height = 480;
  int framebufferWidth;
  int framebufferHeight;
  std::string window_name = "physics Engine";
  GLFWwindow* window;

 public:
  bool Initialize();
  void Update();
  void Render();
  void Exit();

  bool NeedsToCloseWindow();
  GLFWwindow* GetWindow();
};
}  // namespace Engine

void InputCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
void FramebufferSizeCallback(GLFWwindow *window, int width, int height);
void ErrorCallback(int, const char *err_str);