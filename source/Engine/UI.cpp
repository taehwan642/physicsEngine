#include "UI.hpp"

#include "Engine.hpp"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

bool Engine::UI::Initialize(Engine* engine) {
  // Setup Dear ImGui context
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();

  // Setup Dear ImGui style
  ImGui::StyleColorsDark();
  // ImGui::StyleColorsLight();

  // Setup Platform/Renderer backends
  ImGui_ImplGlfw_InitForOpenGL(engine->GetWindow(), true);
  ImGui_ImplOpenGL3_Init("#version 410");
  return true;
}

void Engine::UI::Update() {
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();
  ImGui::Begin("Hello, world!");
  ImGui::Text("Hello, world %d", 123);
  ImGui::Button("Save");
  ImGui::End();
}

void Engine::UI::Render() {
  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Engine::UI::Exit() {
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
}
