
#pragma once
#include "Client_Defines.h"
#include "StateMachine.h"

BEGIN(Client)
class BatPotato_RIG;
class BatPotato_RIGDeath final : public StateMachine<BatPotato_RIG, BATPOTATO_RIG_ANIM>
{
private:
	BatPotato_RIGDeath(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	BatPotato_RIGDeath(const BatPotato_RIGDeath& rhs);
	virtual ~BatPotato_RIGDeath() = default;

public:
	virtual void OnStateEnter() override;
	virtual void OnStateTick(_double TimeDelta) override;
	virtual void OnStateExit() override;

	virtual const _tchar* GetTag() override { return Tag(); }
public:
	static const _tchar* Tag() { return L"BatPotato_RIGDeath"; }
	static BatPotato_RIGDeath* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual StateMachine<BatPotato_RIG, BATPOTATO_RIG_ANIM>* Clone(void* pArg) override;
	virtual void Free() override;
};

END