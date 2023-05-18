#pragma once

#include "Client_Defines.h"
#include "Base.h"

BEGIN(Client)
class CMainApp final : public CBase
{
private:
	CMainApp();
	~CMainApp() = default;

public:
	HRESULT Initialize();
	void Tick(_double TimeDelta);
	HRESULT Render();

private:
	ID3D11Device* m_pDevice = { nullptr };
	ID3D11DeviceContext* m_pDeviceContext = { nullptr };

public:
	static CMainApp* Create();
	void Free() override;
};

END