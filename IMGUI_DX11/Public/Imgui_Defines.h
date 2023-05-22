#pragma once

#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include <d3d11.h>
#include <tchar.h>

namespace Imgui
{
	const unsigned int g_iWinSizeX = 1280;
	const unsigned int g_iWinSizeY = 800;

	enum LEVELID { LEVEL_STATIC, LEVEL_LOGO, LEVEL_GAMEPLAY, LEVEL_LOADING, LEVEL_END };
	enum TOOL_MODE { MAP_TOOL, UNIT_TOOL, COLLISION_TOOL, MODE_END };
}

extern HINSTANCE g_hInst;
extern HWND g_hWnd;
extern ID3D11Device* g_pd3dDevice;
extern ID3D11DeviceContext* g_pd3dDeviceContext;
extern IDXGISwapChain* g_pSwapChain;
extern UINT g_ResizeWidth;
extern UINT	g_ResizeHeight;
extern ID3D11RenderTargetView* g_mainRenderTargetView;

using namespace Imgui;