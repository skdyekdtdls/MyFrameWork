
#pragma once
#include "Client_Defines.h"
#include "StateMachine.h"

BEGIN(Client)
class Clint;
class ClintSkill02 final : public StateMachine<Clint, CLINT_ANIM>
{
private:
	ClintSkill02(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	ClintSkill02(const ClintSkill02& rhs);
	virtual ~ClintSkill02() = default;

public:
	virtual void OnStateEnter() override;
	virtual void OnStateTick(_double TimeDelta) override;
	virtual void OnStateExit() override;

public:
	static const _tchar* Tag() { return L"ClintSkill02"; }
	static ClintSkill02* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual StateMachine<Clint, CLINT_ANIM>* Clone(void* pArg) override;
	virtual void Free() override;
};

END