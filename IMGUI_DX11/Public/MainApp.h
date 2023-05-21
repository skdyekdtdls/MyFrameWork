#pragma once

#include "Imgui_Defines.h"
#include "Base.h"

BEGIN(Engine)
class CRenderer;
class CGameInstance;
END

BEGIN(Imgui)

class CMainApp final : public CBase
{
private:
	CMainApp();
	virtual ~CMainApp() = default;

public:
	HRESULT Initialize();
	void Tick(_double TimeDelta);
	HRESULT Render();

private:
	HRESULT Initialize_Imgui();
	HRESULT Open_Level(LEVELID eLevelIndex); // 일단 넣어놓음..
private:
	ID3D11Device* m_pDevice = { nullptr };
	ID3D11DeviceContext* m_pContext = { nullptr };

private:
	CGameInstance* m_pGameInstance = { nullptr };
	class ImguiManager* m_pImguiMgr = { nullptr };
	CRenderer* m_pRenderer = { nullptr };

public:
	HRESULT Ready_Prototype_Component_For_Static();

public:
	static CMainApp* Create();
	virtual void Free() override;
};

END