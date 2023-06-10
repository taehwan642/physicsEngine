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

#include "Time.hpp"

void Engine::Time::Update() {
  currentTime = glfwGetTime();
  timeDiff = currentTime - prevTime;
  counter++;
  if (timeDiff >= 1.0 / maxFPS) {
    FPS = (1.0 / timeDiff) * counter;
    ms = (timeDiff / counter) * 1000;
    prevTime = currentTime;
    counter = 0;
  }
}

double Engine::Time::GetFPS() { return FPS; }