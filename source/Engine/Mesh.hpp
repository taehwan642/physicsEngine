#pragma once

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

#include <Vector3.hpp>
#include <string>
#include <vector>

namespace Engine {
struct Vertex {
  math::Vector3 position;
  // math::Vector3 normal;
};

class Mesh {
 private:
  std::vector<Vertex> vertices;
  std::vector<unsigned int> indices;
  GLuint vertexBufferObject;
  GLuint vertexArraysObject;
  GLuint elementBuffer;

 public:
  bool Initialize(const std::string& fileName);
  void Render();
  void Exit();
};
}  // namespace Engine