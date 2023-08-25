
#pragma once
#include "Client_Defines.h"
#include "StateMachine.h"

BEGIN(Client)
class CannonSpider;
class CannonSpiderRun final : public StateMachine<CannonSpider, CANNON_SPIDER_ANIM>
{
private:
	CannonSpiderRun(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CannonSpiderRun(const CannonSpiderRun& rhs);
	virtual ~CannonSpiderRun() = default;

public:
	virtual void OnStateEnter() override;
	virtual void OnStateTick(_double TimeDelta) override;
	virtual void OnStateExit() override;

	virtual void OnCollision(CCollider::COLLISION_INFO tCollisionInfo, _double TimeDelta) override;

	virtual const _tchar* GetTag() override { return Tag(); }
public:
	static const _tchar* Tag() { return L"CannonSpiderRun"; }
	static CannonSpiderRun* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual StateMachine<CannonSpider, CANNON_SPIDER_ANIM>* Clone(void* pArg) override;
	virtual void Free() override;
};

END