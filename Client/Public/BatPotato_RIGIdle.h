
#pragma once
#include "Client_Defines.h"
#include "StateMachine.h"

BEGIN(Client)
class BatPotato_RIG;
class BatPotato_RIGIdle final : public StateMachine<BatPotato_RIG, BATPOTATO_RIG_ANIM>
{
private:
	BatPotato_RIGIdle(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	BatPotato_RIGIdle(const BatPotato_RIGIdle& rhs);
	virtual ~BatPotato_RIGIdle() = default;

public:
	virtual void OnStateEnter() override;
	virtual void OnStateTick(_double TimeDelta) override;
	virtual void OnStateExit() override;

	virtual const _tchar* GetTag() override { return Tag(); }

private:
	_double m_TimeAcc = { 0.0 };
public:
	static const _tchar* Tag() { return L"BatPotato_RIGIdle"; }
	static BatPotato_RIGIdle* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual StateMachine<BatPotato_RIG, BATPOTATO_RIG_ANIM>* Clone(void* pArg) override;
	virtual void Free() override;
};

END