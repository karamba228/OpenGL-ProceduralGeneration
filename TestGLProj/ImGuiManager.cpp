#include "ImGuiManager.h"
#include <GL/freeglut.h>

#include <iostream>


ImGuiManager::ImGuiManager()
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    // Setup Platform/Renderer backends
    ImGui_ImplGLUT_Init();
    ImGui_ImplOpenGL3_Init();
}

void ImGuiManager::shutdown() {
    // Shutdown ImGui
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGLUT_Shutdown();
    ImGui::DestroyContext();
}

void ImGuiManager::update() {
	// Rendering
	ImGui::Render();

	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void ImGuiManager::startFrame(const char* name) {
	ImGuiIO& io = ImGui::GetIO();
	static ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	// Set the ImGui delta time
	io.DeltaTime = 1.0f / 60.0f;

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGLUT_NewFrame();

	ImGui::Begin(name);
}

void ImGuiManager::endFrame() {
	ImGui::End();
}

bool ImGuiManager::isUsingMouse() {
	ImGuiIO& io = ImGui::GetIO();
	return io.WantCaptureMouse;
}

bool ImGuiManager::isUsingKeyboard() {
	ImGuiIO& io = ImGui::GetIO();
	return io.WantCaptureKeyboard;
}