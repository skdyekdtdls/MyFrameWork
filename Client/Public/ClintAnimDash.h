#pragma once

#include "ClintAnimState.h"

BEGIN(Client)
class ClintAnimDash final : public ClintAnimState
{
private:
	ClintAnimDash(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~ClintAnimDash() = default;

public:
	virtual HRESULT Initialize(Clint* pOwner) override;
	virtual void Tick(_double TimeDelta) override;
	virtual void Late_Tick(_double TimeDelta) override;

public:
	static ClintAnimDash* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, Clint* pClint);
	virtual void Free() override;
};

END