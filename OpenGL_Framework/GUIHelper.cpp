#include "GUIHelper.h"

void GUIHelper::init(float width, float height)
{
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.DisplaySize.x = width;
	io.DisplaySize.y = height;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

	ImGui_ImplFreeGLUT_Init();
	ImGui_ImplOpenGL3_Init();
}

void GUIHelper::update(float deltaTime, float mouseX, float mouseY, bool leftMouseDown, bool rightMouseDown)
{
	ImGuiIO& io = ImGui::GetIO();
	io.DeltaTime = deltaTime;
	io.MousePos = ImVec2(mouseX, mouseY);
	io.MouseDown[0] = leftMouseDown;
	io.MouseDown[1] = rightMouseDown;
}
