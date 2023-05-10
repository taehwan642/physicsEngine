#pragma once

#include <Shader.hpp>
#include <Transform.hpp>

namespace Engine {
class GameObject {
 private:
 public:
  virtual bool Initialize();
  virtual void Update();
  virtual void Render();
};
}  // namespace Engine