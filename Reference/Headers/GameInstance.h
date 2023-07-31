#pragma once

#include "Component_Manager.h"
#include "Light_Manager.h"
#include "PipeLine.h"
#include "Input_Device.h"
#include "CollisionMgr.h"
#include "Layer.h"
#include "Font_Manager.h"

BEGIN(Engine)
class CGraphic_Device;
class CInput_Device;
class CObject_Manager;
class CLevel_Manager;
class CLevel;
class CGameObject;
class CTimer_Manager;
class CLight_Manager;
class Frustum;
class CTarget_Manager;
class ENGINE_DLL CGameInstance final : public CBase
{
	DECLARE_SINGLETON(CGameInstance)
public:
	CGameInstance();
	~CGameInstance() = default;
public:
	HRESULT Initialize_Engine(_uint iNumLevels, const GRAPHICDESC & GraphicDesc, ID3D11Device** ppDevice, ID3D11DeviceContext** ppDeviceContext);
	void Tick_Engine(_double TimeDelta);

public: /* For Graphic_Device*/
	void ResizeBuffer(_uint & g_ResizeWidth, _uint & g_ResizeHeight);
	HRESULT Clear_BackBuffer_View(_float4 vClearColor);
	HRESULT Clear_DepthStencil_View();
	HRESULT Present();
	_uint2 GetViewPortSize(ID3D11DeviceContext * pContext);
public: /* For Input_Device*/
	_byte	Get_DIKeyState(_ubyte ubyKeyID);
	_byte	Get_DIMouseState(CInput_Device::MOUSEKEYSTATE eMouseID);
	_long	Get_DIMouseMove(CInput_Device::MOUSEMOVESTATE eMouseMoveID);
	_bool	Mouse_Down(CInput_Device::MOUSEKEYSTATE eMouseID);
	_bool	Mouse_Pressing(CInput_Device::MOUSEKEYSTATE eMouseID);
	_bool	Mouse_Up(CInput_Device::MOUSEKEYSTATE eMouseID);
	bool	Key_Pressing(_ubyte ubyKey);
	bool	Key_Down(_ubyte ubyKey);
	bool	Key_Up(_ubyte ubyKey);

public: /* For Level_Manager */
	HRESULT Open_Level(_uint iLevelIndex, CLevel * pNewLevel);
	const _uint& Get_CurLevelIndex();
	const _uint& Get_NextLevelIndex();
	void Set_NextLevelIndex(_uint iIndex);

public: /* For Object_Manager*/
	HRESULT Add_Prototype(const _tchar * pPrototypeTag, CGameObject * pPrototype);
	CGameObject* Add_GameObject(_uint iLevelIndex, const _tchar * pPrototypeTag, const _tchar * pLayerTag, void* pArg = nullptr);
	HRESULT Delete_GameObject(_uint iLevelIndex, const _tchar * pLayerTag, string strName);
	void Clear_LevelResources(_uint iLevelIndex);
	CLayer* Find_LayerByName(_uint iLevelIndex, const _tchar * pLayerTag);
	CGameObject* Get_GameObject(_uint iLevelIndex, const _tchar * pLayerTag, string strName);
	CComponent* Get_ComponentOfClone(_uint iLevelIndex, const _tchar * pLayerTag, string pCloneObjName, const _tchar * pCloneComName);
	_uint GetNumLayers(_uint iLevelIndex);
	LAYERS* GetLayers();
	CGameObject* Clone_GameObject(const _tchar * pPrototypeTag, void* pArg);
	void AddToLayer(_uint iLevelIndex, const _tchar * pLayerTag, CGameObject * pGameObject);
	unordered_map<const _tchar*, CLayer*>::iterator LayerBegin(_uint iLevelIndex);
	unordered_map<const _tchar*, CLayer*>::iterator LayerEnd(_uint iLevelIndex);

	void Serialization(HANDLE hFile, DWORD & dwByte, _uint iLevelIndex);
	void Deserialization(HANDLE hFile, DWORD & dwByte, _uint iLevelIndex);

public: /* For Timer Manager */
	_double Get_Timer(const _tchar * pTimerTag);
	void Set_Timer(const _tchar * pTimerTag);
	HRESULT Ready_Timer(const _tchar * pTimerTag);

public: /* For Component Manager */
	HRESULT Add_Prototype(_uint iLevelIndex, const _tchar * pPrototypeTag, CComponent * pPrototype);
	CComponent* Clone_Component(_uint iLevelIndex, const _tchar * pPrototypeTag, void* pArg = nullptr);
	CComponent* Get_ProtoComponent(_uint iLevelIndex, const _tchar * pProtoTag);

public: /* For PipeLine */
	_float4	Get_CamPositionFloat4() const;
	_vector	Get_CamPositionVector();
	void Set_Transform(CPipeLine::D3DTRANSFORMSTATE eTransformState, _fmatrix TransformStateMatrix);
	_matrix Get_TransformMatrix(CPipeLine::D3DTRANSFORMSTATE eTransformState);
	_float4x4 Get_TransformFloat4x4(CPipeLine::D3DTRANSFORMSTATE eTransformState);
	_matrix Get_TransformMatrix_Inverse(CPipeLine::D3DTRANSFORMSTATE eTransformState);
	_float4x4 Get_TransformFloat4x4_Inverse(CPipeLine::D3DTRANSFORMSTATE eTransformState);

public:
	_bool isIn_WorldSpace(_fvector vWorldPos, _float fRange);

public: /* For Light_Manager */
	const CLight::LIGHTDESC* Get_Light(_uint iIndex);
	HRESULT Add_Lights(const CLight::LIGHTDESC & LightDesc);

public: /* For Collision_Manager */
	void Add_ColliderGroup(CCollider* pCollider, COLL_GROUP eCollGroup);
	
public: /* For Font_Manager */
	HRESULT Add_Fonts(ID3D11Device * pDevice, ID3D11DeviceContext * pContext,
		const _tchar * pFontTag, const _tchar * pFontFilePath);
	HRESULT Render_Font(const _tchar * pFontTag, const _tchar * pText, const _float2 & Position, _fvector vColor = XMVectorSet(1.f, 1.f, 1.f, 1.f), float fRotation = 0.f, const _float2 & vOrigin = _float2(0.f, 0.f), _float fScale = 1.f);
private:
	CGraphic_Device* m_pGraphic_Device = { nullptr };
	CInput_Device* m_pInput_Device = { nullptr };
	CLevel_Manager* m_pLevel_Manager = { nullptr };
	CObject_Manager* m_pObject_Manager = { nullptr };
	CTimer_Manager* m_pTimer_Manager = { nullptr };
	CComponent_Manager* m_pComponent_Manager = { nullptr };
	CPipeLine*			m_pPipeLine = { nullptr };
	Frustum*			m_pFrustum = { nullptr };
	CLight_Manager*		m_pLight_Manager = { nullptr };
	CollisionMgr*		m_pCollision_Manager = { nullptr };
	CTarget_Manager*	m_pTarget_Manager = { nullptr };
	CFont_Manager*		m_pFont_Manager = { nullptr };

public:
	static void Release_Engine();
	void Free() override;
};

END