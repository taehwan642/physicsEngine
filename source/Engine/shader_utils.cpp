/* Defined before OpenGL and GLUT includes to avoid deprecation messages */
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

#include <array>
#include <iostream>
#include <optional>

#include "logs.h"
#include "shader_utils.h"

ShaderUtils::Program::~Program() noexcept {
  if (vertexShader.has_value()) {
    glDeleteShader(vertexShader.value());
  }
  if (vertexShader.has_value()) {
    glDeleteShader(fragmentShader.value());
  }
  if (registered && program.has_value()) {
    glDeleteProgram(program.value());
  }
}

auto ShaderUtils::Program::registerShader(const ShaderUtils::Type shader_type,
                                          const char *shader_source) -> bool {
  int success = {};
  std::array<char, 1024> errorMessage = {};

  bool isFragmentShader =
      shader_type == ShaderUtils::Type::FRAGMENT_SHADER_TYPE;

  auto real_shader_type =
      !isFragmentShader ? GL_VERTEX_SHADER : GL_FRAGMENT_SHADER;

  auto shader = glCreateShader(real_shader_type);
  // Now, pass the shaders
  glShaderSource(shader, 1, &shader_source, NULL);
  // And now, compile them
  glCompileShader(shader);

  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
  if (success == 0) {
    glGetShaderInfoLog(shader, 1024, NULL, errorMessage.data());
    if (isFragmentShader) {
      error("Fragment shader compilation error : " << errorMessage.data());
    } else {
      error("Vertex shader compilation error : " << errorMessage.data());
    }

    return false;
  }

  if (isFragmentShader) {
    fragmentShader = shader;
  } else {
    vertexShader = shader;
  }
  return true;
}

auto ShaderUtils::Program::registerProgram(bool erase_if_registered) -> bool {
  if (registered && (!erase_if_registered || !program.has_value())) {
    error("program is already registered");
    return false;
  }
  if (registered && erase_if_registered) {
    glDeleteProgram(program.value());
    registered = false;
  }
  if (!vertexShader.has_value() || !fragmentShader.has_value()) {
    error("cannot compile program without vertex and fragment shaders");
    return false;
  }
  int success = {};
  std::array<char, 1024> errorMessage = {};
  const unsigned int vertexShaderValue = vertexShader.value();
  const unsigned int fragmentShaderValue = fragmentShader.value();

  program = glCreateProgram();
  const unsigned int programValue = program.value();

  glAttachShader(programValue, vertexShaderValue);
  glAttachShader(programValue, fragmentShaderValue);
  glLinkProgram(programValue);

  glGetProgramiv(programValue, GL_LINK_STATUS, &success);
  if (success == 0) {
    glGetProgramInfoLog(programValue, 1024, NULL, errorMessage.data());
    error("Shader linking error: " << errorMessage.data());
    return false;
  }

  // We can now delete our vertex and fragment shaders
  glDeleteShader(vertexShaderValue);
  glDeleteShader(fragmentShaderValue);
  glUseProgram(programValue);
  registered = true;

  return true;
}

auto ShaderUtils::Program::getProgram() const -> std::optional<unsigned int> {
  return program;
}

auto ShaderUtils::Program::programIsRegistered() const -> bool {
  return registered;
}