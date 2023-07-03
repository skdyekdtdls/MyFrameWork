#pragma once
#include <windows.h>

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

	enum LEVELID { LEVEL_STATIC, LEVEL_IMGUI, LEVEL_LOGO, LEVEL_GAMEPLAY, LEVEL_LOADING, LEVEL_END};
	
	enum class CLINT_ANIM
	{ 
		DASH,// = 27,
		DEATH,// = 29,
		GRANADE,// = 50,
		HIT,// = 53,
		IDLE,// = 64,
		MVP,// = 74,
		RUN,// = 89,
		SKILL_01,// = 109,
		SKILL_02,// = 122,
		WEAPONCHANGE,// = 152,
		CLINT_ANIM_END
	};

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

