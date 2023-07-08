#pragma once

#include "ClintState.h"

BEGIN(Client)
class ClintDash final : public ClintState
{
private:
	ClintDash(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, Clint* pClint);
	virtual ~ClintDash() = default;

public:
	virtual void OnStateEnter() override;
	virtual void OnStateTick(double TimeDelta) override;
	virtual void OnStateExit() override;

public:
	static const _tchar* Tag() { return L"ClintDash"; }
	static ClintDash* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, Clint* pClint);
	virtual void Free() override;
};

END