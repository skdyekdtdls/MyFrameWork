#pragma once

#include "Base.h"
#include "Imgui_Defines.h"

BEGIN(Engine)
class CGameInstance;
END

BEGIN(Imgui)
class ImguiManager;
class ImWindow : public CBase
{
public:
	ImWindow();
	virtual ~ImWindow() = default;

	virtual HRESULT Tick(_double TimeDelta) = 0;

public:
	_bool m_bShowWindow;

protected:
	ImguiManager* m_pImguiMgr = { nullptr };
	CGameInstance* m_pGameInstance = { nullptr };
public:
	virtual void Free() override;
};

END