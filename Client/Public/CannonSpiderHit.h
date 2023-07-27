
#pragma once
#include "Client_Defines.h"
#include "StateMachine.h"

BEGIN(Client)
class CannonSpider;
class CannonSpiderHit final : public StateMachine<CannonSpider, CANNON_SPIDER_ANIM>
{
private:
	CannonSpiderHit(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CannonSpiderHit(const CannonSpiderHit& rhs);
	virtual ~CannonSpiderHit() = default;

public:
	virtual void OnStateEnter() override;
	virtual void OnStateTick(_double TimeDelta) override;
	virtual void OnStateExit() override;

	virtual void OnCollision(CCollider::COLLISION_INFO tCollisionInfo, _double TimeDelta) override;

	virtual const _tchar* GetTag() override { return Tag(); }
public:
	static const _tchar* Tag() { return L"CannonSpiderHit"; }
	static CannonSpiderHit* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual StateMachine<CannonSpider, CANNON_SPIDER_ANIM>* Clone(void* pArg) override;
	virtual void Free() override;
};

END
