#pragma once

#include "Light.h"

BEGIN(Engine)

class CLight_Manager final : public CBase
{
	DECLARE_SINGLETON(CLight_Manager)
private:
	CLight_Manager();
	~CLight_Manager() = default;

public:
	const CLight::LIGHTDESC* Get_Light(_uint iIndex);

public:
	HRESULT Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	HRESULT Add_Lights(const CLight::LIGHTDESC& LightDesc);

private:
	list<CLight*>	m_Lights;

	ID3D11Device* m_pDevice = { nullptr };
	ID3D11DeviceContext* m_pContext = { nullptr };

public:
	virtual void Free(void) override;
};

END