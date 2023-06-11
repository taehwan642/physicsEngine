#pragma once

#include "Component.hpp"
#include "Matrix4x4.hpp"
#include "Vector3.hpp"

namespace Engine {
class Transform : public Component {
 private:
  math::Matrix4x4 transformMatrix;
  math::Vector3 position;
  math::Vector3 rotation;
  math::Vector3 scale;

 public:
  void SetPosition(math::Vector3 newPosition);
  void SetRotation(math::Vector3 newRotation);
  void SetScale(math::Vector3 newScale);
  math::Matrix4x4 GetTransformMatrix();

  virtual bool Initialize() override;
  virtual void Update() override;
  virtual void Exit() override;
};
}  // namespace Engine
