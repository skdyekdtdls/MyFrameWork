#include "..\Public\SpiderHit.h"
#include "GameInstance.h"
#include "Spider.h"

SpiderHit::SpiderHit(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: StateMachine<Spider, SPIDER_ANIM>(pDevice, pContext)
{
}

SpiderHit::SpiderHit(const SpiderHit& rhs)
	: StateMachine<Spider, SPIDER_ANIM>(rhs)
{
}

void SpiderHit::OnStateEnter()
{
	__super::OnStateEnter();

	SetAnimIndex(SPIDER_ANIM_END, LOWER);
}

void SpiderHit::OnStateTick(_double TimeDelta)
{
	__super::OnStateTick(TimeDelta);

	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);
	CModel* pModel = static_cast<CModel*>(m_pOwner->Get_Component(L"Com_Model"));
	CTransform* pTransform = static_cast<CTransform*>(m_pOwner->Get_Component(L"Com_Transform"));

	Safe_Release(pGameInstance);
}

void SpiderHit::OnStateExit()
{
	__super::OnStateExit();
}

void SpiderHit::OnCollision(CCollider::COLLISION_INFO tCollisionInfo, _double TimeDelta)
{
	// 플레이어랑 레이랑 충돌하면 공격상태로 전환
	if (tCollisionInfo.pMyCollider == m_pOwner->GetComponent<Raycast>() &&
		tCollisionInfo.pOtherGameObject->GetInfo().wstrKey == TEXT("Prototype_GameObject_Clint"))
	{
		TransitionTo(L"SpiderAttack");
	}
}

StateMachine<Spider, SPIDER_ANIM>* SpiderHit::Clone(void* pArg)
{
	__super::Initialize(pArg);
	return new SpiderHit(*this);
}

SpiderHit* SpiderHit::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	return new SpiderHit(pDevice, pContext);
}

void SpiderHit::Free()
{
	__super::Free();
}


