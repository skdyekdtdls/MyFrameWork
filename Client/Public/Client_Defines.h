#pragma once
#include <windows.h>
#include "Client_Enum.h"
#include "ClientInstance.h"
#ifdef _DEBUG
#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui.h"
#include "ImGuiFileDialog.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"

#endif

namespace Client
{
	const unsigned int g_iWinSizeX = 1280;
	const unsigned int g_iWinSizeY = 720;
}

extern HINSTANCE g_hInst;
extern HWND g_hWnd;

using namespace Client;

#ifdef _DEBUG
enum IMWIN_MODE {
	OBJ_TOOL_MODE,
	MAP_TOOL_MODE,
	CAMERA_TOOL_MODE,
	ANIM_TOOL_MODE,
	EFFECT_TOOL_MODE,
	UI_TOOL_MODE,
	LIGHT_TOOL_MODE,
	MODE_END
};
#endif // DEBUG

