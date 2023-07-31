
#pragma once
#include "Client_Defines.h"
#include "StateMachine.h"

BEGIN(Client)
class Spider;
class SpiderAppear final : public StateMachine<Spider, SPIDER_ANIM>
{
private:
	SpiderAppear(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	SpiderAppear(const SpiderAppear& rhs);
	virtual ~SpiderAppear() = default;

public:
	virtual void OnStateEnter() override;
	virtual void OnStateTick(_double TimeDelta) override;
	virtual void OnStateExit() override;

	virtual void OnCollision(CCollider::COLLISION_INFO tCollisionInfo, _double TimeDelta) override;

	virtual const _tchar* GetTag() override { return Tag(); }
public:
	static const _tchar* Tag() { return L"SpiderAppear"; }
	static SpiderAppear* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual StateMachine<Spider, SPIDER_ANIM>* Clone(void* pArg) override;
	virtual void Free() override;
};

END