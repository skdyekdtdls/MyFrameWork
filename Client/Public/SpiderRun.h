
#pragma once
#include "Client_Defines.h"
#include "StateMachine.h"

BEGIN(Client)
class Spider;
class SpiderRun final : public StateMachine<Spider, SPIDER_ANIM>
{
private:
	SpiderRun(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	SpiderRun(const SpiderRun& rhs);
	virtual ~SpiderRun() = default;

public:
	virtual void OnStateEnter() override;
	virtual void OnStateTick(_double TimeDelta) override;
	virtual void OnStateExit() override;

	virtual void OnCollision(CCollider::COLLISION_INFO tCollisionInfo, _double TimeDelta) override;

	virtual const _tchar* GetTag() override { return Tag(); }
public:
	static const _tchar* Tag() { return L"SpiderRun"; }
	static SpiderRun* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual StateMachine<Spider, SPIDER_ANIM>* Clone(void* pArg) override;
	virtual void Free() override;
};

END


#include "..\Public\SpiderRun.h"
#include "GameInstance.h"
#include "Spider.h"

SpiderRun::SpiderRun(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: StateMachine<Spider, SPIDER_ANIM>(pDevice, pContext)
{
}

SpiderRun::SpiderRun(const SpiderRun& rhs)
	: StateMachine<Spider, SPIDER_ANIM>(rhs)
{
}

void SpiderRun::OnStateEnter()
{
	__super::OnStateEnter();

	SetAnimIndex(SPIDER_ANIM_END, LOWER);
}

void SpiderRun::OnStateTick(_double TimeDelta)
{
	__super::OnStateTick(TimeDelta);

	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);
	CModel* pModel = static_cast<CModel*>(m_pOwner->Get_Component(L"Com_Model"));
	CTransform* pTransform = static_cast<CTransform*>(m_pOwner->Get_Component(L"Com_Transform"));

	Safe_Release(pGameInstance);
}

void SpiderRun::OnStateExit()
{
	__super::OnStateExit();
}

void SpiderRun::OnCollision(CCollider::COLLISION_INFO tCollisionInfo, _double TimeDelta)
{
	// 플레이어랑 레이랑 충돌하면 공격상태로 전환
	if (tCollisionInfo.pMyCollider == m_pOwner->GetComponent<Raycast>() &&
		tCollisionInfo.pOtherGameObject->GetInfo().wstrKey == TEXT("Prototype_GameObject_Clint"))
	{
		TransitionTo(L"SpiderAttack");
	}
}

StateMachine<Spider, SPIDER_ANIM>* SpiderRun::Clone(void* pArg)
{
	__super::Initialize(pArg);
	return new SpiderRun(*this);
}

SpiderRun* SpiderRun::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	return new SpiderRun(pDevice, pContext);
}

void SpiderRun::Free()
{
	__super::Free();
}


