#pragma once
#include <windows.h>

#ifdef _DEBUG
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include <d3d11.h>

#endif

namespace Client
{
	const unsigned int g_iWinSizeX = 1280;
	const unsigned int g_iWinSizeY = 720;

	enum LEVELID { LEVEL_STATIC, LEVEL_IMGUI, LEVEL_LOGO, LEVEL_GAMEPLAY, LEVEL_LOADING, LEVEL_END};
#ifdef _DEBUG
	enum MODE { OBJECT_PLACE_MODE, NAVIGATION_MESH_MODE, TRANSFORM_MODE, MODE_END };
#endif
}

extern HINSTANCE g_hInst;
extern HWND g_hWnd;

using namespace Client;