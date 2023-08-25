#include "pch.hpp"
#include "DearImGuiLayer.hpp"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>
#include "Events/MouseEvents.hpp"
#include "Events/EventUtils.hpp"

DearImGuiLayer::DearImGuiLayer()
{
	GLFWwindow* glfwWindow = glfwGetCurrentContext();

	ImGui::CreateContext();
	ImGui::StyleColorsLight();
	ImGui_ImplGlfw_InitForOpenGL(glfwWindow, true);
	ImGui_ImplOpenGL3_Init();

	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	io.IniFilename = nullptr;
}

DearImGuiLayer::~DearImGuiLayer()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

void DearImGuiLayer::OnEvent(Event& event)
{
	// TODO: Capture all inputs:
	// - Mouse press, release, move, scroll
	// - Key press, release, type
	ImGuiIO& io = ImGui::GetIO();
	if (io.WantCaptureMouseUnlessPopupClose && Events::IsMouseEvent(event))
	{
		event.Handled = true;
	}
	if (io.WantCaptureKeyboard && Events::IsKeyEvent(event))
	{
		event.Handled = true;
	}
}

void DearImGuiLayer::Begin()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

void DearImGuiLayer::End()
{
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	// TODO
	/*
	ImGuiIO& io = ImGui::GetIO();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		GLFWwindow* backup_current_context = glfwGetCurrentContext();
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
		glfwMakeContextCurrent(backup_current_context);
	}
	*/
}