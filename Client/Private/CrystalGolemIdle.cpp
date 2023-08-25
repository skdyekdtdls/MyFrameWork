#include "..\Public\CrystalGolemIdle.h"
#include "GameInstance.h"
#include "CrystalGolem.h"

CrystalGolemIdle::CrystalGolemIdle(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: StateMachine<CrystalGolem, CRYSTAL_GOLEM_ANIM>(pDevice, pContext)
{
}

CrystalGolemIdle::CrystalGolemIdle(const CrystalGolemIdle& rhs)
	: StateMachine<CrystalGolem, CRYSTAL_GOLEM_ANIM>(rhs)
{
}

void CrystalGolemIdle::OnStateEnter()
{
	__super::OnStateEnter();

	SetAnimIndex(CRYSTAL_GOLEM_IDLE, LOWER);
	m_TimeAcc = { 0.0 };
}

void CrystalGolemIdle::OnStateTick(_double TimeDelta)
{
	__super::OnStateTick(TimeDelta);

	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);
	CModel* pModel = static_cast<CModel*>(m_pOwner->Get_Component(L"Com_Model"));
	CTransform* pTransform = static_cast<CTransform*>(m_pOwner->Get_Component(L"Com_Transform"));
	m_TimeAcc += TimeDelta;
	if (m_TimeAcc >= 1.f)
		TransitionTo(L"CrystalGolemRun");

	Safe_Release(pGameInstance);
}

void CrystalGolemIdle::OnStateExit()
{
	__super::OnStateExit();
}

void CrystalGolemIdle::OnCollision(CCollider::COLLISION_INFO tCollisionInfo, _double TimeDelta)
{
	// 플레이어랑 레이랑 충돌하면 공격상태로 전환
	if (tCollisionInfo.pMyCollider == m_pOwner->GetComponent<Raycast>() &&
		tCollisionInfo.pOtherGameObject->GetInfo().wstrKey == TEXT("Prototype_GameObject_Clint"))
	{
		TransitionTo(L"CrystalGolemAttack");
	}
}

StateMachine<CrystalGolem, CRYSTAL_GOLEM_ANIM>* CrystalGolemIdle::Clone(void* pArg)
{
	__super::Initialize(pArg);
	return new CrystalGolemIdle(*this);
}

CrystalGolemIdle* CrystalGolemIdle::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	return new CrystalGolemIdle(pDevice, pContext);
}

void CrystalGolemIdle::Free()
{
	__super::Free();
}