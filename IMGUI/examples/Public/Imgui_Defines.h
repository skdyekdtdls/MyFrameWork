#pragma once

#pragma once
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include <d3d11.h>
#include <tchar.h>
#include <windows.h>

namespace Imgui
{
    const unsigned int g_iWinSizeX = 1280;
    const unsigned int g_iWinSizeY = 720;

    enum LEVELID { LEVEL_TOOL, LEVEL_END };
}

extern HINSTANCE g_hInst;
extern HWND g_hWnd;

using namespace Imgui;
