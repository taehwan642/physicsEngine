#include "Scene.hpp"

bool Engine::Scene::Initialize() {
  for (auto& obj : sceneObjects) {
    if (obj->Initialize() == false) return false;
  }
  return true;
}

void Engine::Scene::Update() {
  for (auto& obj : sceneObjects) {
    obj->Update();
  }
}

void Engine::Scene::Render() {
  for (auto& obj : sceneObjects) {
    obj->Render();
  }
}

void Engine::Scene::Exit() { sceneObjects.clear(); }
