
#pragma once
#include "Client_Defines.h"
#include "StateMachine.h"

BEGIN(Client)
class CannonSpider;
class CannonSpiderIdle final : public StateMachine<CannonSpider, CANNON_SPIDER_ANIM>
{
private:
	CannonSpiderIdle(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CannonSpiderIdle(const CannonSpiderIdle& rhs);
	virtual ~CannonSpiderIdle() = default;

public:
	virtual void OnStateEnter() override;
	virtual void OnStateTick(_double TimeDelta) override;
	virtual void OnStateExit() override;

	virtual void OnCollision(CCollider::COLLISION_INFO tCollisionInfo, _double TimeDelta) override;

	virtual const _tchar* GetTag() override { return Tag(); }

private:
	_double m_TimeAcc = { 0.0 };
public:
	static const _tchar* Tag() { return L"CannonSpiderIdle"; }
	static CannonSpiderIdle* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual StateMachine<CannonSpider, CANNON_SPIDER_ANIM>* Clone(void* pArg) override;
	virtual void Free() override;
};

END