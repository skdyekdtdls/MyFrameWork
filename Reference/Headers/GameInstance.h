#pragma once

#include "Base.h"


BEGIN(Engine)
class CGraphic_Device;
class CInput_Device;
class CObject_Manager;
class CComponent_Manager;

class ENGINE_DLL CGameInstance final : public CBase
{
	DECLARE_SINGLETON(CGameInstance)
public:
	CGameInstance();
	~CGameInstance() = default;
public:
	HRESULT Initialize_Engine(const GRAPHICDESC & GraphicDesc, ID3D11Device** ppDevice, ID3D11DeviceContext** ppDeviceContext);
	void Tick_Engine(_double TimeDelta);

public: /* For Graphic_Device*/
	HRESULT	Ready_Graphic_Device(HWND hWnd, GRAPHICDESC::WINMODE eWinMode
		, _uint iWinCX, _uint iWinCY
		, _Inout_ ID3D11Device * *ppDeviceOut, _Inout_ ID3D11DeviceContext * *ppDeviceContextOut);
	HRESULT Clear_BackBuffer_View(_float4 vClearColor);
	HRESULT Clear_DepthStencil_View();
	HRESULT Present();

public:
	static void Release_Engine();
private:
	CGraphic_Device* m_pGraphic_Device = { nullptr };

private:
	void Free() override;
};

END