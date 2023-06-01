#pragma once

#include "Component_Manager.h"
#include "Light_Manager.h"
#include "PipeLine.h"
#include "Input_Device.h"

BEGIN(Engine)
class CGraphic_Device;
class CInput_Device;
class CObject_Manager;
class CLevel_Manager;
class CLevel;
class CGameObject;
class CTimer_Manager;
class CLight_Manager;

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

public: /* For Input_Device*/
	_byte Get_DIKeyState(_ubyte ubyKeyID);
	_byte Get_DIMouseState(CInput_Device::MOUSEKEYSTATE eMouseID);
	_long Get_DIMouseMove(CInput_Device::MOUSEMOVESTATE eMouseMoveID);

public: /* For Level_Manager */
	HRESULT Open_Level(_uint iLevelIndex, CLevel * pNewLevel);

public: /* For Object_Manager*/
	HRESULT Add_Prototype(const _tchar * pPrototypeTag, CGameObject * pPrototype);
	HRESULT Add_GameObject(_uint iLevelIndex, const _tchar * pPrototypeTag, const _tchar * pLayerTag, void* pArg = nullptr);
	void Clear_LevelResources(_uint iLevelIndex);

public: /* For Timer Manager */
	_double Get_Timer(const _tchar * pTimerTag);
	void Set_Timer(const _tchar * pTimerTag);
	HRESULT Ready_Timer(const _tchar * pTimerTag);

public: /* For Component Manager */
	HRESULT Add_Prototype(_uint iLevelIndex, const _tchar * pPrototypeTag, CComponent * pPrototype);
	CComponent* Clone_Component(_uint iLevelIndex, const _tchar * pPrototypeTag, void* pArg = nullptr);

public: /* For PipeLine */
	_float4	Get_CamPosition() const;
	void Set_Transform(CPipeLine::D3DTRANSFORMSTATE eTransformState, _fmatrix TransformStateMatrix);
	_matrix Get_TransformMatrix(CPipeLine::D3DTRANSFORMSTATE eTransformState);
	_float4x4 Get_TransformFloat4x4(CPipeLine::D3DTRANSFORMSTATE eTransformState);
	_matrix Get_TransformMatrix_Inverse(CPipeLine::D3DTRANSFORMSTATE eTransformState);
	_float4x4 Get_TransformFloat4x4_Inverse(CPipeLine::D3DTRANSFORMSTATE eTransformState);

public: /* For Light_Manager */
	const CLight::LIGHTDESC* Get_Light(_uint iIndex);
	HRESULT Add_Lights(const CLight::LIGHTDESC & LightDesc);

private:
	CGraphic_Device* m_pGraphic_Device = { nullptr };
	CInput_Device* m_pInput_Device = { nullptr };
	CLevel_Manager* m_pLevel_Manager = { nullptr };
	CObject_Manager* m_pObject_Manager = { nullptr };
	CTimer_Manager* m_pTimer_Manager = { nullptr };
	CComponent_Manager* m_pComponent_Manager = { nullptr };
	CPipeLine*			m_pPipeLine = { nullptr };
	CLight_Manager*		m_pLight_Manager = { nullptr };
public:
	static void Release_Engine();
	void Free() override;
};

END