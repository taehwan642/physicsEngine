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

/**
 * @brief Load the shaders, in order to display the result
 *
 * @param erase_if_program_registered Allow to erase the shader if it exists
 * @return true The shader has been successfully registered
 * @return false The shader has not been registered, due to an error
 */
const bool loadShaderProgram(const bool erase_if_program_registered);

/*
 * Callback to handle the "reload" event, once the user pressed the 'r' key.
 */
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

/*
 * Initializes the window and viewport via GLFW.
 * The viewport takes the all window.
 * If an error happens, the function returns `NULL` but **does not** free /
 * terminate the GLFW library. Then, do not forget to call `glfwTerminate` if
 * this function returns `NULL`.
 */
GLFWwindow *initializeWindow() {
  // Minimum target is OpenGL 4.1
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
  // Easy reload
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

/**
 * @brief Returns the all file, as a string, which the file path has been passed
 * as parameter
 *
 * @param path The path of the file
 * @return The content of the file, as a string (read all file)
 */
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
  const MathsUtils::vertex vertices[3] = {
      {0.0f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f},
      {0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f},
      {-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f}};

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

  // Specify color attribute -> 3 as offset
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,
                        MathsUtils::VERTEX_ELEMENTS_NB * sizeof(float),
                        (GLvoid *)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  /* END OF DRAWING */

  while (!glfwWindowShouldClose(window)) {
    // Render
    glClearColor(0.5, 0.5, 0.5, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(shader_utils.getProgram().value());
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);
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