#include "..\Public\Queen_MoggothDead.h"
#include "GameInstance.h"
#include "Queen_Moggoth.h"

Queen_MoggothDead::Queen_MoggothDead(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: StateMachine<Queen_Moggoth, QUEEN_MOGGOTH_ANIM>(pDevice, pContext)
{
}

Queen_MoggothDead::Queen_MoggothDead(const Queen_MoggothDead& rhs)
	: StateMachine<Queen_Moggoth, QUEEN_MOGGOTH_ANIM>(rhs)
{
}

void Queen_MoggothDead::OnStateEnter()
{
	__super::OnStateEnter();

	SetAnimIndex(QUEEN_MOGGOTH_P3_DEATH, LOWER);
}

void Queen_MoggothDead::OnStateTick(_double TimeDelta)
{
	__super::OnStateTick(TimeDelta);

	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);
	CModel* pModel = static_cast<CModel*>(m_pOwner->Get_Component(L"Com_Model"));
	CTransform* pTransform = static_cast<CTransform*>(m_pOwner->Get_Component(L"Com_Transform"));

	if (pModel->IsAnimationFinished())
	{
		Single->AddExp(100.f);
	}

	Safe_Release(pGameInstance);
}

void Queen_MoggothDead::OnStateExit()
{
	__super::OnStateExit();
}

void Queen_MoggothDead::OnCollision(CCollider::COLLISION_INFO tCollisionInfo, _double TimeDelta)
{
	// 플레이어랑 레이랑 충돌하면 공격상태로 전환
	if (tCollisionInfo.pMyCollider == m_pOwner->GetComponent<Raycast>() &&
		tCollisionInfo.pOtherGameObject->GetInfo().wstrKey == TEXT("Prototype_GameObject_Clint"))
	{
		TransitionTo(L"Queen_MoggothAttack");
	}
}

StateMachine<Queen_Moggoth, QUEEN_MOGGOTH_ANIM>* Queen_MoggothDead::Clone(void* pArg)
{
	__super::Initialize(pArg);
	return new Queen_MoggothDead(*this);
}

Queen_MoggothDead* Queen_MoggothDead::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	return new Queen_MoggothDead(pDevice, pContext);
}

void Queen_MoggothDead::Free()
{
	__super::Free();
}


