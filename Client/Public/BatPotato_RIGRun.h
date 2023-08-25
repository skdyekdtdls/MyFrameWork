
#pragma once
#include "Client_Defines.h"
#include "StateMachine.h"

BEGIN(Client)
class BatPotato_RIG;
class BatPotato_RIGRun final : public StateMachine<BatPotato_RIG, BATPOTATO_RIG_ANIM>
{
private:
	BatPotato_RIGRun(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	BatPotato_RIGRun(const BatPotato_RIGRun& rhs);
	virtual ~BatPotato_RIGRun() = default;

public:
	virtual void OnStateEnter() override;
	virtual void OnStateTick(_double TimeDelta) override;
	virtual void OnStateExit() override;

	virtual void OnCollision(CCollider::COLLISION_INFO tCollisionInfo, _double TimeDelta) override;

	virtual const _tchar* GetTag() override { return Tag(); }
public:
	static const _tchar* Tag() { return L"BatPotato_RIGRun"; }
	static BatPotato_RIGRun* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual StateMachine<BatPotato_RIG, BATPOTATO_RIG_ANIM>* Clone(void* pArg) override;
	virtual void Free() override;
};

END