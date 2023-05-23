#pragma once

#include "Imgui_Defines.h"
#include "Level.h"

BEGIN(Imgui)
class CLevel_GamePlay final : public CLevel
{
private:
	CLevel_GamePlay(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CLevel_GamePlay() = default;

public:
	virtual HRESULT Initialize() override;
	virtual void Tick(_double TimeDelta) override;
	virtual void Late_Tick(_double TimeDelta) override;
	virtual HRESULT Render() override;

public:
	static CLevel_GamePlay* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext);
	void Free() override;
};

END