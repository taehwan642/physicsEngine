#pragma once

#include <GameObject.hpp>
#include <memory>
#include <vector>
namespace Engine {
class Scene {
 private:
  std::vector<std::shared_ptr<GameObject>> sceneObjects;

 public:
  bool Initialize();
  void Update();
  void Render();
  void Exit();
};
}  // namespace Engine