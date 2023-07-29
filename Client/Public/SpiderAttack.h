
#pragma once
#include "Client_Defines.h"
#include "StateMachine.h"

BEGIN(Client)
class Spider;
class SpiderAttack final : public StateMachine<Spider, SPIDER_ANIM>
{
private:
	SpiderAttack(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	SpiderAttack(const SpiderAttack& rhs);
	virtual ~SpiderAttack() = default;

public:
	virtual void OnStateEnter() override;
	virtual void OnStateTick(_double TimeDelta) override;
	virtual void OnStateExit() override;

	virtual void OnCollision(CCollider::COLLISION_INFO tCollisionInfo, _double TimeDelta) override;

	virtual const _tchar* GetTag() override { return Tag(); }
public:
	static const _tchar* Tag() { return L"SpiderAttack"; }
	static SpiderAttack* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual StateMachine<Spider, SPIDER_ANIM>* Clone(void* pArg) override;
	virtual void Free() override;
};

END


#include "..\Public\SpiderAttack.h"
#include "GameInstance.h"
#include "Spider.h"

SpiderAttack::SpiderAttack(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: StateMachine<Spider, SPIDER_ANIM>(pDevice, pContext)
{
}

SpiderAttack::SpiderAttack(const SpiderAttack& rhs)
	: StateMachine<Spider, SPIDER_ANIM>(rhs)
{
}

void SpiderAttack::OnStateEnter()
{
	__super::OnStateEnter();

	SetAnimIndex(SPIDER_ANIM_END, LOWER);
}

void SpiderAttack::OnStateTick(_double TimeDelta)
{
	__super::OnStateTick(TimeDelta);

	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);
	CModel* pModel = static_cast<CModel*>(m_pOwner->Get_Component(L"Com_Model"));
	CTransform* pTransform = static_cast<CTransform*>(m_pOwner->Get_Component(L"Com_Transform"));

	Safe_Release(pGameInstance);
}

void SpiderAttack::OnStateExit()
{
	__super::OnStateExit();
}

void SpiderAttack::OnCollision(CCollider::COLLISION_INFO tCollisionInfo, _double TimeDelta)
{
	// 플레이어랑 레이랑 충돌하면 공격상태로 전환
	if (tCollisionInfo.pMyCollider == m_pOwner->GetComponent<Raycast>() &&
		tCollisionInfo.pOtherGameObject->GetInfo().wstrKey == TEXT("Prototype_GameObject_Clint"))
	{
		TransitionTo(L"SpiderAttack");
	}
}

StateMachine<Spider, SPIDER_ANIM>* SpiderAttack::Clone(void* pArg)
{
	__super::Initialize(pArg);
	return new SpiderAttack(*this);
}

SpiderAttack* SpiderAttack::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	return new SpiderAttack(pDevice, pContext);
}

void SpiderAttack::Free()
{
	__super::Free();
}


