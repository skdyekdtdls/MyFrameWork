
#pragma once
#include "Client_Defines.h"
#include "StateMachine.h"

BEGIN(Client)
class Clint;
class ClintUltimate01 final : public StateMachine<Clint, CLINT_ANIM>
{
private:
	ClintUltimate01(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	ClintUltimate01(const ClintUltimate01& rhs);
	virtual ~ClintUltimate01() = default;

public:
	virtual void OnStateEnter() override;
	virtual void OnStateTick(_double TimeDelta) override;
	virtual void OnStateExit() override;

private:
	_double m_TimeAcc = { 0.0 };
	_double m_Duration = { 6.0 };

public:
	static const _tchar* Tag() { return L"ClintUltimate01"; }
	static ClintUltimate01* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual StateMachine<Clint, CLINT_ANIM>* Clone(void* pArg) override;
	virtual void Free() override;
};

END