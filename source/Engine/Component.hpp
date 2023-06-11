#pragma once

namespace Engine {
class Component {
 private:
 public:
  virtual bool Initialize();
  virtual void Update();
  virtual void Exit();
};
}  // namespace Engine
