#ifndef _SHADER_UTILS_H
#define _SHADER_UTILS_H

#include <optional>

namespace ShaderUtils {

enum Type {
  FRAGMENT_SHADER_TYPE,
  VERTEX_SHADER_TYPE,
};

struct Program {
 private:
  /**
   * @brief The vertex shader ID, as an optional
   */
  std::optional<unsigned int> vertexShader = std::nullopt;

  /**
   * @brief The fragment shader ID, as an optional
   */
  std::optional<unsigned int> fragmentShader = std::nullopt;

  /**
   * @brief The GPU program ID (or shader ID), as an optional
   */
  std::optional<unsigned int> program = std::nullopt;

  /**
   * @brief Stores if the shader has been registered or not
   */
  bool registered = false;

 public:
  /**
   * @brief Constructor
   */
  Program() = default;

  /**
   * @brief Destructor
   */
  ~Program() noexcept;

  /**
   * @brief Register a shader
   *
   * @param shader_type The type: fragment or vertex
   * @param shader_source The source code as a string
   * @return true The shader has been successfully registered
   * @return false The shader has not been registered - error is logged
   */
  auto registerShader(const Type shader_type, const char *shader_source)
      -> bool;

  /**
   * @brief Register the GPU program (or shader), after compilation
   * of the fragment and vertex shaders.
   *
   * @param erase_if_registered Erase the current program if one is already
   * registered
   * @return true The shader has been successfully registered
   * @return false The shader has not been successfully registered - error is
   * logged
   */
  auto registerProgram(bool erase_if_registered) -> bool;

  /**
   * @brief Returns the GPU program ID object, as optional
   *
   * @return std::optional<unsigned int>
   */
  auto getProgram() const -> std::optional<unsigned int>;

  /**
   * @brief Returns if the GPU program object has been registered or not
   */
  auto programIsRegistered() const -> bool;
};

}  // namespace ShaderUtils

#endif /* SHADER_UTILS_H */