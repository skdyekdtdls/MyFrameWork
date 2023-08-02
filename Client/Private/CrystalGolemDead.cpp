#include "..\Public\CrystalGolemDead.h"
#include "GameInstance.h"
#include "CrystalGolem.h"

CrystalGolemDead::CrystalGolemDead(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: StateMachine<CrystalGolem, CRYSTAL_GOLEM_ANIM>(pDevice, pContext)
{
}

CrystalGolemDead::CrystalGolemDead(const CrystalGolemDead& rhs)
	: StateMachine<CrystalGolem, CRYSTAL_GOLEM_ANIM>(rhs)
{
}

void CrystalGolemDead::OnStateEnter()
{
	__super::OnStateEnter();

	SetAnimIndex(CRYSTAL_GOLEM_DEATH, LOWER);
}

void CrystalGolemDead::OnStateTick(_double TimeDelta)
{
	__super::OnStateTick(TimeDelta);

	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);
	CModel* pModel = static_cast<CModel*>(m_pOwner->Get_Component(L"Com_Model"));
	CTransform* pTransform = static_cast<CTransform*>(m_pOwner->Get_Component(L"Com_Transform"));

	if (pModel->IsAnimationFinished())
	{
		Single->AddExp(100.f);
		m_pOwner->SetDead();
	}
	Safe_Release(pGameInstance);
}

void CrystalGolemDead::OnStateExit()
{
	__super::OnStateExit();
}

void CrystalGolemDead::OnCollision(CCollider::COLLISION_INFO tCollisionInfo, _double TimeDelta)
{
	// 플레이어랑 레이랑 충돌하면 공격상태로 전환
	if (tCollisionInfo.pMyCollider == m_pOwner->GetComponent<Raycast>() &&
		tCollisionInfo.pOtherGameObject->GetInfo().wstrKey == TEXT("Prototype_GameObject_Clint"))
	{
		TransitionTo(L"CrystalGolemAttack");
	}
}

StateMachine<CrystalGolem, CRYSTAL_GOLEM_ANIM>* CrystalGolemDead::Clone(void* pArg)
{
	__super::Initialize(pArg);
	return new CrystalGolemDead(*this);
}

CrystalGolemDead* CrystalGolemDead::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	return new CrystalGolemDead(pDevice, pContext);
}

void CrystalGolemDead::Free()
{
	__super::Free();
}


