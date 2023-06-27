#pragma once

namespace Engine
{
	struct INFO
	{
		INFO() : ID(0){}
		~INFO() { wstrName.clear(); }

		void Save(HANDLE hFile, DWORD& dwByte)
		{
			_uint iNameLength = static_cast<_uint>(wstrName.size()) + 1;
			_uint iNameKey = static_cast<_uint>(wstrKey.size()) + 1;

			WriteVoid(&iNameLength, sizeof(_uint));
			WriteVoid(&iNameKey, sizeof(_uint));
			WriteVoid(&ID, sizeof(_uint));
			if (iNameLength > 0) {
				WriteVoid(&wstrName[0], sizeof(_tchar) * iNameLength);
			}			
			if (iNameKey > 0) {
				WriteVoid(&wstrKey[0], sizeof(_tchar) * iNameKey);
			}
		}

		void Load(HANDLE hFile, DWORD& dwByte) {
			_uint iNameLength;
			_uint iNameKey;

			ReadVoid(&iNameLength, sizeof(_uint));
			ReadVoid(&iNameKey, sizeof(_uint));
			ReadVoid(&ID, sizeof(_uint));

			if (iNameLength > 0) {
				wstrName.resize(iNameLength);
				ReadVoid(&wstrName[0], sizeof(_tchar) * iNameLength);
			}
			else {
				wstrName.clear();
			}

			if (iNameKey > 0) {
				wstrKey.resize(iNameKey);
				ReadVoid(&wstrKey[0], sizeof(_tchar) * iNameKey);
			}
			else {
				wstrKey.clear();
			}
		}

		wstring wstrName;
		wstring wstrKey;
		_uint ID;
	};

	struct PICK_DESC
	{
		bool operator==(const PICK_DESC& rhs) const {
			if (vPickPos.x != rhs.vPickPos.x) return false;
			if (vPickPos.y != rhs.vPickPos.y) return false;
			if (vPickPos.z != rhs.vPickPos.z) return false;
			if (vPickPos.w != rhs.vPickPos.w) return false;
			if (!FloatEqual(fDist, rhs.fDist)) return false;
			if (pPickedObject != rhs.pPickedObject) return false;

			return true;
		}

		bool operator!=(const PICK_DESC& rhs) const {
			return !(*this == rhs);
		}

		XMFLOAT4 vPickPos = { 0.f, 0.f, 0.f, 1.f };
		float	 fDist;
		class CGameObject* pPickedObject = { nullptr };
	};

	struct CELL_PICK_DESC
	{
		bool operator==(const CELL_PICK_DESC& rhs) const {
			if (vPickPos.x != rhs.vPickPos.x) return false;
			if (vPickPos.y != rhs.vPickPos.y) return false;
			if (vPickPos.z != rhs.vPickPos.z) return false;
			if (vPickPos.w != rhs.vPickPos.w) return false;
			if (!FloatEqual(fDist, rhs.fDist)) return false;
			if (pPickedCell != rhs.pPickedCell) return false;

			return true;
		}

		bool operator!=(const CELL_PICK_DESC& rhs) const {
			return !(*this == rhs);
		}

		XMFLOAT4 vPickPos = { 0.f, 0.f, 0.f, 1.f };
		float	 fDist;
		class CCell* pPickedCell = { nullptr };
		_int	iVertexIndex = { -1 };
	};

	//struct VERTEX_PICK_DESC
	//{
	//	XMFLOAT4 vVertexPos = { 0.f, 0.f, 0.f, 1.f };
	//	float	 fDist;
	//	class CCell* pPickedCell = { nullptr };
	//};

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