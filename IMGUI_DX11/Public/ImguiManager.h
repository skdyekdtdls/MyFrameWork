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

	HRESULT Tick();

public:
	_float4 clear_color = _float4(0.45f, 0.55f, 0.60f, 1.00f);
private:
	map<string, int> a;
	CGameInstance* m_pGameInstance = { nullptr };
public:
	virtual void Free() override;
};

END