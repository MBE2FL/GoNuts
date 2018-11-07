#pragma once
#include "imgui/imgui.h"
//#include "imgui/imgui_impl.h"
#include <vector>

using std::vector;

class GUIHelper
{
public:
	static void init(float width, float height);
	static void update(float deltaTime, float mouseX, float mouseY,
		bool leftMouseDown, bool rightMouseDown);
};