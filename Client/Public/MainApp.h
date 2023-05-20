#pragma once

#include "Client_Defines.h"
#include "Base.h"

BEGIN(Engine)
class CGameInstance;
class CRenderer;
END

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
	HRESULT Open_Level(LEVELID eLevelIndex);

	HRESULT Ready_Prototype_Component_For_Static();

private: /* For Component */
	CRenderer* m_pRenderer = { nullptr };

private:
	CGameInstance* m_pGameInstance = { nullptr };
	ID3D11Device* m_pDevice = { nullptr };
	ID3D11DeviceContext* m_pContext = { nullptr };
	
public:
	static CMainApp* Create();
	void Free() override;
};

END