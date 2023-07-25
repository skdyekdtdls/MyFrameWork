
#pragma once
#include "Client_Defines.h"
#include "StateMachine.h"

BEGIN(Client)
class Clint;
class ClintSkill01 final : public StateMachine<Clint, CLINT_ANIM>
{
private:
	ClintSkill01(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	ClintSkill01(const ClintSkill01& rhs);
	virtual ~ClintSkill01() = default;

public:
	virtual void OnStateEnter() override;
	virtual void OnStateTick(_double TimeDelta) override;
	virtual void OnStateExit() override;

public:
	static const _tchar* Tag() { return L"ClintSkill01"; }
	static ClintSkill01* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual StateMachine<Clint, CLINT_ANIM>* Clone(void* pArg) override;
	virtual void Free() override;
};

END





