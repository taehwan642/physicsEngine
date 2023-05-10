#pragma once

namespace Engine {
class Engine;
class UI {
 private:
 public:
  bool Initialize(Engine* engine);
  void Update();
  void Render();
  void Exit();
};
}  // namespace Engine