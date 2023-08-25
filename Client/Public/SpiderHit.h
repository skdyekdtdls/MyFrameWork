
#pragma once
#include "Client_Defines.h"
#include "StateMachine.h"

BEGIN(Client)
class Spider;
class SpiderHit final : public StateMachine<Spider, SPIDER_ANIM>
{
private:
	SpiderHit(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	SpiderHit(const SpiderHit& rhs);
	virtual ~SpiderHit() = default;

public:
	virtual void OnStateEnter() override;
	virtual void OnStateTick(_double TimeDelta) override;
	virtual void OnStateExit() override;

	virtual void OnCollision(CCollider::COLLISION_INFO tCollisionInfo, _double TimeDelta) override;

	virtual const _tchar* GetTag() override { return Tag(); }
public:
	static const _tchar* Tag() { return L"SpiderHit"; }
	static SpiderHit* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual StateMachine<Spider, SPIDER_ANIM>* Clone(void* pArg) override;
	virtual void Free() override;
};

END