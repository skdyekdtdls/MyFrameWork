#pragma once

#include "Base.h"
#include "Imgui_Defines.h"

BEGIN(Imgui)
class ImguiManager;
class ImWindow : public CBase
{
public:
	ImWindow();
	virtual ~ImWindow() = default;

	virtual HRESULT Tick() = 0;

public:
	_bool m_bShowWindow;

protected:
	ImguiManager* m_pImguiMgr;
public:
	virtual void Free() override;
};

END