
#pragma once
#include "Client_Defines.h"
#include "StateMachine.h"

BEGIN(Client)
class BatPotato_RIG;
class BatPotato_RIGAttack final : public StateMachine<BatPotato_RIG, BATPOTATO_RIG_ANIM>
{
private:
	BatPotato_RIGAttack(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	BatPotato_RIGAttack(const BatPotato_RIGAttack& rhs);
	virtual ~BatPotato_RIGAttack() = default;

public:
	virtual void OnStateEnter() override;
	virtual void OnStateTick(_double TimeDelta) override;
	virtual void OnStateExit() override;

	virtual const _tchar* GetTag() override { return Tag(); }
public:
	static const _tchar* Tag() { return L"BatPotato_RIGAttack"; }
	static BatPotato_RIGAttack* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual StateMachine<BatPotato_RIG, BATPOTATO_RIG_ANIM>* Clone(void* pArg) override;
	virtual void Free() override;
};

END





