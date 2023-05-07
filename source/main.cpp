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

#include <Engine.hpp>
#include <Mesh.hpp>
#include <Scene.hpp>
#include <fstream>
#include <iostream>
#include <memory>

#include "Matrix4x4.hpp"
#include "Vector2.hpp"
#include "Vector3.hpp"
#include "logs.h"
#include "maths_utils.h"
#include "shader_utils.h"

auto shader_utils = ShaderUtils::Program{};

const bool loadShaderProgram(const bool erase_if_program_registered);

inline auto readFile(const std::string_view path) -> const std::string {
  // Avoid dynamic allocation: read the 4096 first bytes
  constexpr auto read_size = std::size_t(4096);
  auto stream = std::ifstream(path.data());
  stream.exceptions(std::ios_base::badbit);

  auto out = std::string();
  auto buf = std::string(read_size, '\0');
  while (stream.read(&buf[0], read_size)) {
    out.append(buf, 0, stream.gcount());
  }
  out.append(buf, 0, stream.gcount());
  return out;
}

const bool loadShaderProgram(const bool erase_if_program_registered = true) {
  const std::string basicVertexShaderSource =
      readFile("Shader/vertex_shader.glsl");
  const std::string basicFragmentShaderSource =
      readFile("Shader/fragment_shader.glsl");

  if (!shader_utils.registerShader(ShaderUtils::Type::VERTEX_SHADER_TYPE,
                                   basicVertexShaderSource.c_str())) {
    error("failed to register the vertex shader...");
    return false;
  }

  if (!shader_utils.registerShader(ShaderUtils::Type::FRAGMENT_SHADER_TYPE,
                                   basicFragmentShaderSource.c_str())) {
    error("failed to register the fragment shader...");
    return false;
  }

  if (!shader_utils.registerProgram(erase_if_program_registered)) {
    error("failed to register the program...");
    return false;
  }
  return true;
}

int main(void) {
  std::unique_ptr<Engine::Engine> engine = std::make_unique<Engine::Engine>();
  engine->Initialize();
  glEnable(GL_DEPTH_TEST);
  std::shared_ptr<Engine::Scene> scene = std::make_shared<Engine::Scene>();
  engine->AddScene(scene);

  engine->EnterScene(0);

  // load shader

  // load model
  std::unique_ptr<Engine::Mesh> mesh = std::make_unique<Engine::Mesh>();
  mesh->Initialize("Test");

  // attach model to object

  if (!loadShaderProgram(false)) {
    error("can't load the shaders to initiate the program");
    engine->Exit();
    return -1;
  }
  /* END OF SHADER PART */

  /* DRAW THE TRIANGLE */

  while (!engine->NeedsToCloseWindow()) {
    static float yaw = 0.0f;
    yaw += 0.01f;
    engine->Update();
    math::Matrix4x4 transform = math::Matrix4x4::CreateIdentityMatrix();
    transform = transform * math::Matrix4x4::CreateScaleMatrix(
                                math::Vector3(0.1, 0.1, 0.1));

    transform = transform * math::Matrix4x4::CreateRotationYawMatrix(yaw);
    transform = transform * math::Matrix4x4::CreateTranslationMatrix(
                                math::Vector3(0, 0, -0.3f));
    transform = transform * math::Matrix4x4::CreatePerspectiveMatrix(
                                0.785398,
                                engine->GetWidth() / (float)engine->GetHeight(),
                                0.1f, 100.0f);

    transform.ToString();
    engine->Render();

    // Render
    glClearColor(1.0, 0.5, 0.5, 1.0);
    glClear(GL_COLOR_BUFFER_BIT |
            GL_DEPTH_BUFFER_BIT);  // also clear the depth buffer now!
    glUseProgram(shader_utils.getProgram().value());

    unsigned int transformLoc =
        glGetUniformLocation(shader_utils.getProgram().value(), "transform");
    glUniformMatrix4fv(transformLoc, 1, GL_TRUE, &transform.element[0][0]);

    mesh->Render();
    glUseProgram(0);

    // Poll for and process events
    glfwPollEvents();
    // Swap front and back buffers
    glfwSwapBuffers(engine->GetWindow());
  }

  // ... here, the user closed the window
  engine->Exit();
  mesh->Exit();
  return 0;
}