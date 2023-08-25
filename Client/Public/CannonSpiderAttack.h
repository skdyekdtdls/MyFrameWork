
#pragma once
#include "Client_Defines.h"
#include "StateMachine.h"

BEGIN(Client)
class CannonSpider;
class CannonSpiderAttack final : public StateMachine<CannonSpider, CANNON_SPIDER_ANIM>
{
private:
	CannonSpiderAttack(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CannonSpiderAttack(const CannonSpiderAttack& rhs);
	virtual ~CannonSpiderAttack() = default;

public:
	virtual void OnStateEnter() override;
	virtual void OnStateTick(_double TimeDelta) override;
	virtual void OnStateExit() override;

	virtual void OnCollision(CCollider::COLLISION_INFO tCollisionInfo, _double TimeDelta) override;

	virtual const _tchar* GetTag() override { return Tag(); }
public:
	static const _tchar* Tag() { return L"CannonSpiderAttack"; }
	static CannonSpiderAttack* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual StateMachine<CannonSpider, CANNON_SPIDER_ANIM>* Clone(void* pArg) override;
	virtual void Free() override;
};

END





