#include "..\Public\Queen_MoggothIdle.h"
#include "GameInstance.h"
#include "Queen_Moggoth.h"

Queen_MoggothIdle::Queen_MoggothIdle(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: StateMachine<Queen_Moggoth, QUEEN_MOGGOTH_ANIM>(pDevice, pContext)
{
}

Queen_MoggothIdle::Queen_MoggothIdle(const Queen_MoggothIdle& rhs)
	: StateMachine<Queen_Moggoth, QUEEN_MOGGOTH_ANIM>(rhs)
{
}

void Queen_MoggothIdle::OnStateEnter()
{
	__super::OnStateEnter();

	SetAnimIndex(QUEEN_MOGGOTH_P1_IDLE, LOWER);
	m_TimeAcc = { 0.0 };
}

void Queen_MoggothIdle::OnStateTick(_double TimeDelta)
{
	__super::OnStateTick(TimeDelta);

	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);
	CModel* pModel = static_cast<CModel*>(m_pOwner->Get_Component(L"Com_Model"));
	CTransform* pTransform = static_cast<CTransform*>(m_pOwner->Get_Component(L"Com_Transform"));

	m_TimeAcc += TimeDelta;

	if (m_TimeAcc >= 1.0)
		TransitionTo(L"Queen_MoggothWalk");

	Safe_Release(pGameInstance);
}

void Queen_MoggothIdle::OnStateExit()
{
	__super::OnStateExit();
}

void Queen_MoggothIdle::OnCollision(CCollider::COLLISION_INFO tCollisionInfo, _double TimeDelta)
{
	// 플레이어랑 레이랑 충돌하면 공격상태로 전환
	if (tCollisionInfo.pMyCollider == m_pOwner->GetComponent<Raycast>() &&
		tCollisionInfo.pOtherGameObject->GetInfo().wstrKey == TEXT("Prototype_GameObject_Clint"))
	{
		TransitionTo(L"Queen_MoggothAttack");
	}
}

StateMachine<Queen_Moggoth, QUEEN_MOGGOTH_ANIM>* Queen_MoggothIdle::Clone(void* pArg)
{
	__super::Initialize(pArg);
	return new Queen_MoggothIdle(*this);
}

Queen_MoggothIdle* Queen_MoggothIdle::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	return new Queen_MoggothIdle(pDevice, pContext);
}

void Queen_MoggothIdle::Free()
{
	__super::Free();
}


