#pragma once

#include "ClintState.h"

BEGIN(Client)
class ClintIdle final : public ClintState
{
private:
	ClintIdle(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, Clint* pClint);
	virtual ~ClintIdle() = default;

public:
	virtual void OnStateEnter() override;
	virtual void OnStateTick(_double TimeDelta) override;
	virtual void OnStateExit() override;

public:
	static const _tchar* Tag() { return L"ClintIdle"; }
	static ClintIdle* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, Clint* pClint);
	virtual void Free() override;
};

END