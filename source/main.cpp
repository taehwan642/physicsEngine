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

#include <fstream>
#include <iostream>

#include "Vector2.hpp"
#include "Vector3.hpp"
#include "logs.h"
#include "maths_utils.h"
#include "shader_utils.h"

const size_t WIDTH = 640;
const size_t HEIGHT = 480;
const char *WINDOW_NAME = "OpenGL Explorer";
auto shader_utils = ShaderUtils::Program{};

const bool loadShaderProgram(const bool erase_if_program_registered);

static void KeyCallback(GLFWwindow *window, int key, int scancode, int action,
                        int _mods) {
  if (key == GLFW_KEY_R && action == GLFW_PRESS) {
    debug("reloading...");
    loadShaderProgram(true);
  }

  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    glfwSetWindowShouldClose(window, GLFW_TRUE);
}

void FramebufferSizeCallback(GLFWwindow *window, int width, int height) {
  glViewport(0, 0, width, height);
}

GLFWwindow *initializeWindow() {
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, WINDOW_NAME, NULL, NULL);
  if (!window) {
    error("window creation failed");
    return NULL;
  }

  // Close the window as soon as the Escape key has been pressed
  glfwSetKeyCallback(window, KeyCallback);
  // Makes the window context current
  glfwMakeContextCurrent(window);
#ifdef _WIN64
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    error("could not start GLAD");
    return NULL;
  }
#endif
  // Enable the viewport
  // cannot use gl function before initilize (defined region code)
  int width, height;
  glfwGetFramebufferSize(window, &width, &height);
  glViewport(0, 0, width, height);

  glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback);

  return window;
}

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

void ErrorCallback(int, const char *err_str) {
  std::cout << "GLFW Error: " << err_str << std::endl;
}

int main(void) {
  glfwSetErrorCallback(ErrorCallback);
  // Initialize the lib
  if (!glfwInit()) {
    error("could not start GLFW3");
    return -1;
  }

  math::Vector2 v2 = math::Vector2(1, 1);
  math::Vector3 v3 = math::Vector3(1, 1, 1);
  std::cout << v2.LengthSquared() << " " << v2.Length() << std::endl;
  std::cout << v3.LengthSquared() << " " << v3.Length() << std::endl;

  GLFWwindow *window = initializeWindow();
  if (!window) {
    glfwTerminate();
    return -1;
  }

  // Note: Once you have a current OpenGL context, you can use OpenGL normally
  // get version info
  const GLubyte *renderer = glGetString(GL_RENDERER);
  const GLubyte *version = glGetString(GL_VERSION);
  info("Renderer: " << renderer);
  info("OpenGL version supported: " << version);

  if (!loadShaderProgram(false)) {
    error("can't load the shaders to initiate the program");
    glfwTerminate();
    return -1;
  }
  /* END OF SHADER PART */

  /* DRAW THE TRIANGLE */
  const MathsUtils::vertex vertices[4] = {
      {-1.0f, 1.0f, 0.0f},
      {-1.0f, -1.0f, 0.0f},
      {1.0f, -1.0f, 0.0f},
      {1.0f, 1.0f, 0.0f}};

  // Vertex Buffer Object = VBO
  GLuint VBO = {};
  glGenBuffers(1, &VBO);

  // Something failed when generating buffers
  if (glGetError() != GL_NO_ERROR) {
    error("error when generating buffers");
    glDeleteBuffers(1, &VBO);  // TODO: Needed?
    glfwTerminate();
    return -1;
  }
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER,
               MathsUtils::getNbElements(vertices) * sizeof(float), vertices,
               GL_STATIC_DRAW | GL_MAP_READ_BIT);

  // Vertex Arrays Object = VAO
  GLuint VAO = {};
  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);

  // Specify position attribute -> 0 as offset
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
                        MathsUtils::VERTEX_ELEMENTS_NB * sizeof(float),
                        (GLvoid *)0);
  glEnableVertexAttribArray(0);

  glBindBuffer(GL_ARRAY_BUFFER, 0);

  unsigned int indices[6] = {0, 1, 2, 0, 2, 3};

  GLuint elementbuffer;
  glGenBuffers(1, &elementbuffer);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
  /* END OF DRAWING */

  auto aspect_ratio = WIDTH / (float)HEIGHT;

  float viewport_height = 2.0;
  // ratio = width / height
  // width = ratio * height
  float viewport_width = aspect_ratio * viewport_height;
  float focal_length = 1.0;
  math::Vector3 origin = math::Vector3(0, 0, 0);
  math::Vector3 horizontal = math::Vector3(viewport_width, 0, 0);
  math::Vector3 vertical = math::Vector3(0.0f, viewport_height, 0.0);
  math::Vector3 lower_left_corner = origin - horizontal / 2.0f - vertical / 2.0f - math::Vector3(0, 0, focal_length);

  math::Vector3 sphereOrigin = math::Vector3(0, 0, -1);
  float radius = 0.5f;

  while (!glfwWindowShouldClose(window)) {
    // Render
    glClearColor(1.0, 0.5, 0.5, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(shader_utils.getProgram().value());
    
    int uniformLowerLeftLocation = glGetUniformLocation(shader_utils.getProgram().value(), "lower_left_corner");
    glUniform3f(uniformLowerLeftLocation, lower_left_corner.x_, lower_left_corner.y_, lower_left_corner.z_);

    int uniformHorizonLocation = glGetUniformLocation(shader_utils.getProgram().value(), "horizontal");
    glUniform3f(uniformHorizonLocation, horizontal.x_, horizontal.y_, horizontal.z_);

    int uniformVerticalLocation = glGetUniformLocation(shader_utils.getProgram().value(), "vertical");
    glUniform3f(uniformVerticalLocation, vertical.x_, vertical.y_, vertical.z_);

    int uniformOriginLocation = glGetUniformLocation(shader_utils.getProgram().value(), "origin");
    glUniform3f(uniformOriginLocation, origin.x_, origin.y_, origin.z_);

    int uniformSphereLocation = glGetUniformLocation(shader_utils.getProgram().value(), "sphereOrigin");
    glUniform3f(uniformSphereLocation, sphereOrigin.x_, sphereOrigin.y_, sphereOrigin.z_);

    int uniformRadiusLocation = glGetUniformLocation(shader_utils.getProgram().value(), "radius");
    glUniform1f(uniformRadiusLocation, radius);

    glBindVertexArray(VAO);
    glDrawElements(
    GL_TRIANGLES,      // mode
    6,    // count
    GL_UNSIGNED_INT,   // type
    (void*)0           // element array buffer offset
    );
    glUseProgram(0);

    // Poll for and process events
    glfwPollEvents();
    // Swap front and back buffers
    glfwSwapBuffers(window);
  }

  // ... here, the user closed the window
  glDeleteBuffers(1, &VBO);
  glDeleteVertexArrays(1, &VAO);
  glfwTerminate();
  return 0;
}