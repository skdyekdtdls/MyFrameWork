#pragma once

#include "ClintState.h"

BEGIN(Client)
class ClintRun final : public ClintState
{
private:
	ClintRun(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, Clint* pClint);
	virtual ~ClintRun() = default;

public:
	virtual void OnStateEnter() override;
	virtual void OnStateTick(_double TimeDelta) override;
	virtual void OnStateExit() override;

public:
	static ClintRun* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, Clint* pClint);
	virtual void Free() override;
};

END