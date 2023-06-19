#include "Transform.hpp"

#include "Vector3.hpp"

void Engine::Transform::SetPosition(math::Vector3 newPosition) {
  position = newPosition;
}

void Engine::Transform::SetRotation(math::Vector3 newRotation) {
  rotation = newRotation;
}

void Engine::Transform::SetScale(math::Vector3 newScale) { scale = newScale; }

math::Matrix4x4 Engine::Transform::GetTransformMatrix() {
  return transformMatrix;
}

bool Engine::Transform::Initialize() {
  transformMatrix = math::Matrix4x4::CreateIdentityMatrix();
  return true;
}

void Engine::Transform::Update() {
  transformMatrix = math::Matrix4x4::CreateIdentityMatrix();
  transformMatrix = transformMatrix * math::Matrix4x4::CreateScaleMatrix(scale);

  transformMatrix = transformMatrix *
                    math::Matrix4x4::CreateRotationYawPitchRollMatrix(rotation);
  transformMatrix =
      transformMatrix * math::Matrix4x4::CreateTranslationMatrix(position);
}

void Engine::Transform::Exit() {}
