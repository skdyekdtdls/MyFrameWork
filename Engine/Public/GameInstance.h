#pragma once

#include "Component_Manager.h"

BEGIN(Engine)
class CGraphic_Device;
class CInput_Device;
class CObject_Manager;
class CLevel_Manager;
class CLevel;
class CGameObject;
class CTimer_Manager;

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

private:
	CGraphic_Device* m_pGraphic_Device = { nullptr };
	CLevel_Manager* m_pLevel_Manager = { nullptr };
	CObject_Manager* m_pObject_Manager = { nullptr };
	CTimer_Manager* m_pTimer_Manager = { nullptr };
	CComponent_Manager* m_pComponent_Manager = { nullptr };

public:
	static void Release_Engine();
	void Free() override;
};

END