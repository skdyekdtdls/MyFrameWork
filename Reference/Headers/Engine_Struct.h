#pragma once

namespace Engine
{
	typedef struct tagPickDesc
	{
		XMFLOAT3 vPickPos;
		float	 fDist;
		class CGameObject* pPickedObject = { nullptr };
	}PICK_DESC;

	typedef struct tagGraphicDesc
	{
		enum WINMODE { WM_FULL, WM_WIN, WM_END };
		HWND	hWnd;
		HINSTANCE hInst;
		unsigned int	iViewportSizeX, iViewportSizeY;
		WINMODE		eWinMode;
	}GRAPHICDESC;


	typedef struct ENGINE_DLL tagMeshMaterial
	{
		class CTexture* pMtrlTexture[AI_TEXTURE_TYPE_MAX];
	}MESHMATERIAL;

	typedef struct tagKeyFrame
	{
		XMFLOAT3	vScale;
		XMFLOAT4	vRotation;
		XMFLOAT3	vTranslation;
		double		Time;
	}KEYFRAME;

	typedef struct tagRay
	{
		_float4 vRayOrigin;
		_float4 vRayDir;
	}RAY;

	typedef struct tagVertex_Postion
	{
		_float3 vPosition;
	}VTXPOS;

	typedef struct ENGINE_DLL tagVertex_Position_Decl
	{
		static const unsigned int iNumElements = { 1 };
		static const D3D11_INPUT_ELEMENT_DESC	Elements[iNumElements];
	}VTXPOS_DECL;

	typedef struct tagVertex_Anim_Mesh
	{
		XMFLOAT3	vPosition;
		XMFLOAT3	vNormal;
		XMFLOAT2	vTexCoord;
		XMFLOAT3	vTangent;
		XMUINT4		vBlendIndices;
		XMFLOAT4	vBlendWeights;
	}VTXANIMMESH;

	typedef struct ENGINE_DLL tagVertex_Anim_Mesh_Decl
	{
		static const unsigned int iNumElements = { 6 };
		static const D3D11_INPUT_ELEMENT_DESC	Elements[iNumElements];
	}VTXANIMMESH_DECL;

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

	typedef struct tagVertex_Mesh
	{
		XMFLOAT3	vPosition;
		XMFLOAT3	vNormal;
		XMFLOAT2	vTexCoord;
		XMFLOAT3	vTangent;
	}VTXMESH;

	typedef struct ENGINE_DLL tagVertex_Mesh_Decl
	{
		static const unsigned int iNumElements = { 4 };
		static const D3D11_INPUT_ELEMENT_DESC	Elements[iNumElements];
	}VTXMESH_DECL;

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

}