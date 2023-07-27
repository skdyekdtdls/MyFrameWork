
#pragma once
#include "Client_Defines.h"
#include "StateMachine.h"

BEGIN(Client)
class CannonSpider;
class CannonSpiderSearch final : public StateMachine<CannonSpider, CANNON_SPIDER_ANIM>
{
	enum LOGIC { LOGIC1, LOGIC2, LOGIC3, LOGIC_END};
private:
	CannonSpiderSearch(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CannonSpiderSearch(const CannonSpiderSearch& rhs);
	virtual ~CannonSpiderSearch() = default;

public:
	virtual void OnStateEnter() override;
	virtual void OnStateTick(_double TimeDelta) override;
	virtual void OnStateExit() override;

	virtual void OnCollision(CCollider::COLLISION_INFO tCollisionInfo, _double TimeDelta) override;

	virtual const _tchar* GetTag() override { return Tag(); }

private:
	LOGIC m_eCurLogic = { LOGIC_END };
	
private:
	void RandomLogic();
	void SetAnimIndexByCurLogic();
	void CurLogicUpdate(const _double& TimeDelta);

	void Logic1(const _double& TimeDelta);
	void Logic2(const _double& TimeDelta);
	void Logic3(const _double& TimeDelta);

public:
	static const _tchar* Tag() { return L"CannonSpiderSearch"; }
	static CannonSpiderSearch* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual StateMachine<CannonSpider, CANNON_SPIDER_ANIM>* Clone(void* pArg) override;
	virtual void Free() override;
};

END