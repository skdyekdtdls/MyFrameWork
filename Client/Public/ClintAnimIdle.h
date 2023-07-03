#pragma once

#include "ClintAnimState.h"

BEGIN(Client)
class ClintAnimIdle final : public ClintAnimState
{
private:
	ClintAnimIdle(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~ClintAnimIdle() = default;

public:
	virtual HRESULT Initialize(Clint* pOwner) override;
	virtual void Tick(_double TimeDelta) override;
	virtual void Late_Tick(_double TimeDelta) override;

public:
	static ClintAnimIdle* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, Clint* pClint);
	virtual void Free() override;
};

END