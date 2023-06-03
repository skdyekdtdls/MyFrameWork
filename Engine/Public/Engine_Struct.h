#pragma once

namespace Engine
{
	typedef struct tagGraphicDesc
	{
		enum WINMODE { WM_FULL, WM_WIN, WM_END };
		HWND	hWnd;
		HINSTANCE hInst;
		unsigned int	iViewportSizeX, iViewportSizeY;
		WINMODE		eWinMode;
	}GRAPHICDESC;

	typedef struct tagVertex_Position_Color
	{
		XMFLOAT3	vPosition;
		XMFLOAT4	vColor;
	}VTXPOSCOL;

	typedef struct ENGINE_DLL tagVertex_Position_Color_Declaration
	{
		static const unsigned int	iNumElements = { 2 };
		static const D3D11_INPUT_ELEMENT_DESC	Elements[iNumElements];
	}VTXPOSCOL_DECL;

	typedef struct tagVertex_Position_TexCoord
	{
		XMFLOAT3	vPosition;
		XMFLOAT2	vTexCoord;
	}VTXPOSTEX;

	typedef struct ENGINE_DLL tagVertex_Position_TexCoord_Declaration
	{
		static const unsigned int	iNumElements = { 2 };
		static const D3D11_INPUT_ELEMENT_DESC	Elements[iNumElements];
	}VTXPOSTEX_DECL;


	typedef struct tagVertex_Position_Normal_TexCoord
	{
		XMFLOAT3	vPosition;
		XMFLOAT3	vNormal;
		XMFLOAT2	vTexCoord;
	}VTXPOSNORTEX;

	typedef struct ENGINE_DLL tagVertex_Position_Normal_TexCoord_Declaration
	{
		static const unsigned int iNumElements = { 3 };
		static const D3D11_INPUT_ELEMENT_DESC	Elements[iNumElements];
	}VTXPOSNORTEX_DECL;
}