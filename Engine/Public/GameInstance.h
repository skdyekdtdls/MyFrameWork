#pragma once

#include "Base.h"


BEGIN(Engine)
class CGraphic_Device;
class CInput_Device;
class CObject_Manager;
class CComponent_Manager;
class CLevel_Manager;
class CLevel;
class CGameObject;
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
	HRESULT Clear_BackBuffer_View(_float4 vClearColor);
	HRESULT Clear_DepthStencil_View();
	HRESULT Present();

public: /* For Level_Manager */
	HRESULT Open_Level(_uint iLevelIndex, CLevel * pNewLevel);

public: /* For Object_Manager*/
	HRESULT Add_Prototype(const _tchar * pPrototypeTag, CGameObject * pPrototype);
	HRESULT Add_GameObject(_uint iLevelIndex, const _tchar * pPrototypeTag, const _tchar * pLayerTag, void* pArg = nullptr);
	void Clear_LevelResources(_uint iLevelIndex);

private:
	CGraphic_Device* m_pGraphic_Device = { nullptr };
	CLevel_Manager* m_pLevel_Manager = { nullptr };
	CObject_Manager* m_pObject_Manager = { nullptr };

public:
	static void Release_Engine();
	void Free() override;
};

END