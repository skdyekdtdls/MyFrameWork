#pragma once
#include <windows.h>

namespace Client
{
	const unsigned int g_iWinSizeX = 1280;
	const unsigned int g_iWinSizeY = 720;

	enum LEVELID { LEVEL_STATIC, LEVEL_LOGO, LEVEL_GAMEPLAY, LEVEL_LOADING, LEVEL_END};
}

extern HINSTANCE g_hInstance;
extern HWND g_hWnd;

using namespace Client;