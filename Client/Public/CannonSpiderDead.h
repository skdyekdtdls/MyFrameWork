
#pragma once
#include "Client_Defines.h"
#include "StateMachine.h"

BEGIN(Client)
class CannonSpider;
class CannonSpiderDead final : public StateMachine<CannonSpider, CANNON_SPIDER_ANIM>
{
private:
	CannonSpiderDead(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CannonSpiderDead(const CannonSpiderDead& rhs);
	virtual ~CannonSpiderDead() = default;

public:
	virtual void OnStateEnter() override;
	virtual void OnStateTick(_double TimeDelta) override;
	virtual void OnStateExit() override;

	virtual const _tchar* GetTag() override { return Tag(); }
public:
	static const _tchar* Tag() { return L"CannonSpiderDead"; }
	static CannonSpiderDead* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual StateMachine<CannonSpider, CANNON_SPIDER_ANIM>* Clone(void* pArg) override;
	virtual void Free() override;
};

END
