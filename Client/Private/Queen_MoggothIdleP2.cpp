#include "..\Public\Queen_MoggothIdleP2.h"
#include "GameInstance.h"
#include "Queen_Moggoth.h"

Queen_MoggothIdleP2::Queen_MoggothIdleP2(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: StateMachine<Queen_Moggoth, QUEEN_MOGGOTH_ANIM>(pDevice, pContext)
{
}

Queen_MoggothIdleP2::Queen_MoggothIdleP2(const Queen_MoggothIdleP2& rhs)
	: StateMachine<Queen_Moggoth, QUEEN_MOGGOTH_ANIM>(rhs)
{
}

void Queen_MoggothIdleP2::OnStateEnter()
{
	__super::OnStateEnter();

	SetAnimIndex(QUEEN_MOGGOTH_P2_IDLE, LOWER);
	m_TimeAcc = { 0.0 };
}

void Queen_MoggothIdleP2::OnStateTick(_double TimeDelta)
{
	__super::OnStateTick(TimeDelta);

	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);
	CModel* pModel = static_cast<CModel*>(m_pOwner->Get_Component(L"Com_Model"));
	CTransform* pTransform = static_cast<CTransform*>(m_pOwner->Get_Component(L"Com_Transform"));

	m_TimeAcc += TimeDelta;
	if (m_TimeAcc >= 2.0)
		TransitionTo(L"Queen_MoggothWalkP2");

	Safe_Release(pGameInstance);
}

void Queen_MoggothIdleP2::OnStateExit()
{
	__super::OnStateExit();
}

void Queen_MoggothIdleP2::OnCollision(CCollider::COLLISION_INFO tCollisionInfo, _double TimeDelta)
{

}

StateMachine<Queen_Moggoth, QUEEN_MOGGOTH_ANIM>* Queen_MoggothIdleP2::Clone(void* pArg)
{
	__super::Initialize(pArg);
	return new Queen_MoggothIdleP2(*this);
}

Queen_MoggothIdleP2* Queen_MoggothIdleP2::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	return new Queen_MoggothIdleP2(pDevice, pContext);
}

void Queen_MoggothIdleP2::Free()
{
	__super::Free();
}


