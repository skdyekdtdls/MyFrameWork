#pragma once
#include "Client_Defines.h"
#include "StateMachine.h"

BEGIN(Client)
class Clint;
class ClintDash final : public StateMachine<Clint, CLINT_ANIM>
{
private:
	ClintDash(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	ClintDash(const ClintDash& rhs);
	virtual ~ClintDash() = default;

public:
	virtual void OnStateEnter() override;
	virtual void OnStateTick(_double TimeDelta) override;
	virtual void OnStateExit() override;

public:
	static const _tchar* Tag() { return L"ClintDash"; }
	static ClintDash* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual StateMachine<Clint, CLINT_ANIM>* Clone(void* pArg);
	virtual void Free() override;
};

END