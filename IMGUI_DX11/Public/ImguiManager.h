#pragma once

#include "Base.h"
#include "ImWindow.h"

BEGIN(Engine)
class CGameInstance;
END

BEGIN(Imgui)

class ImguiManager final : public CBase
{
	DECLARE_SINGLETON(ImguiManager)
public:
	ImguiManager();
	~ImguiManager() = default;

	HRESULT Add_ImWindow(const _tchar* tag, ImWindow* pImWindow);
	HRESULT Tick(_double TimeDelta);

public:
	_float4 clear_color = _float4(0.45f, 0.55f, 0.60f, 1.00f);
private:
	unordered_map<const _tchar*, ImWindow*> m_ImWindows;
	CGameInstance* m_pGameInstance = { nullptr };

private:
	ImWindow* Find_ImWindow(const _tchar* tag);

public:
	virtual void Free() override;
};

END