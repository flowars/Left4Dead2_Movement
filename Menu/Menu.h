#pragma once
#define IMGUI_DEFINE_MATH_OPERATORS
#include <string>
#include "Imgui/imgui.h"
#include "Imgui/imgui_impl_dx9.h"
#include "Imgui/imgui_impl_win32.h"

#include "../Core/Globals.h"
#include "Config.h"

inline bool CheckKey(int key)
{
	if (GetAsyncKeyState(key) & 0x8000)
		return true;

	return false;
}