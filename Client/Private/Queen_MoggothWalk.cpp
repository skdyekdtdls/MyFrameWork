#include "..\Public\Queen_MoggothWalk.h"
#include "GameInstance.h"
#include "Queen_Moggoth.h"

Queen_MoggothWalk::Queen_MoggothWalk(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: StateMachine<Queen_Moggoth, QUEEN_MOGGOTH_ANIM>(pDevice, pContext)
{
}

Queen_MoggothWalk::Queen_MoggothWalk(const Queen_MoggothWalk& rhs)
	: StateMachine<Queen_Moggoth, QUEEN_MOGGOTH_ANIM>(rhs)
{
}

void Queen_MoggothWalk::OnStateEnter()
{
	__super::OnStateEnter();

	SetAnimIndex(QUEEN_MOGGOTH_P1_IDLE, LOWER);

	m_ePattern = static_cast<PATTERN>(RandomIntFrom_A_To_B(0, PATTERN_END - 1));
	SetAnimIndexByPattern();
	m_pModel = static_cast<CModel*>(m_pOwner->Get_Component(L"Com_Model"));
	m_pTransform = static_cast<CTransform*>(m_pOwner->Get_Component(L"Com_Transform"));
	m_pNavigation = static_cast<CNavigation*>(m_pOwner->Get_Component(L"Com_Navigation"));
	m_TimeAcc = { 0.0 };
}

void Queen_MoggothWalk::OnStateTick(_double TimeDelta)
{
	__super::OnStateTick(TimeDelta);

	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);	

	_vector vClintPos = Single->GetClintPosition();
	_vector vMyPos = m_pTransform->Get_State(CTransform::STATE_POSITION);
	_vector vTargetVector = vClintPos - vMyPos;
	m_pTransform->Turn(WorldAxisY(), vTargetVector, TimeDelta);

	m_TimeAcc += TimeDelta;

	if (m_TimeAcc >= 2.0)
	{
		TransitionTo(L"Queen_MoggothAttack");
	}

	switch (m_ePattern)
	{
	case Client::Queen_MoggothWalk::WALK_FWD:
		WalkFwd(TimeDelta);
		break;
	case Client::Queen_MoggothWalk::WALK_BACK:
		WalkBack(TimeDelta);
		break;
	case Client::Queen_MoggothWalk::WALK_LEFT:
		WalkLeft(TimeDelta);
		break;
	case Client::Queen_MoggothWalk::WALK_RIGHT:
		WalkRight(TimeDelta);
		break;
	}

	Safe_Release(pGameInstance);
}

void Queen_MoggothWalk::OnStateExit()
{
	__super::OnStateExit();
}

void Queen_MoggothWalk::OnCollision(CCollider::COLLISION_INFO tCollisionInfo, _double TimeDelta)
{

}

StateMachine<Queen_Moggoth, QUEEN_MOGGOTH_ANIM>* Queen_MoggothWalk::Clone(void* pArg)
{
	__super::Initialize(pArg);
	return new Queen_MoggothWalk(*this);
}

void Queen_MoggothWalk::WalkFwd(const _double& TimeDelta)
{
	m_pTransform->Go_Straight(TimeDelta, m_pNavigation);
}

void Queen_MoggothWalk::WalkBack(const _double& TimeDelta)
{
	m_pTransform->Go_Backward(TimeDelta, m_pNavigation);
}

void Queen_MoggothWalk::WalkLeft(const _double& TimeDelta)
{
	m_pTransform->Go_Left(TimeDelta, m_pNavigation);
}

void Queen_MoggothWalk::WalkRight(const _double& TimeDelta)
{
	m_pTransform->Go_Right(TimeDelta, m_pNavigation);
}

void Queen_MoggothWalk::SetAnimIndexByPattern()
{
	switch (m_ePattern)
	{
	case Client::Queen_MoggothWalk::WALK_FWD:
		SetAnimIndex(QUEEN_MOGGOTH_P1_WALK_FWD);
		break;
	case Client::Queen_MoggothWalk::WALK_BACK:
		SetAnimIndex(QUEEN_MOGGOTH_P1_WALK_BACK);
		break;
	case Client::Queen_MoggothWalk::WALK_LEFT:
		SetAnimIndex(QUEEN_MOGGOTH_P1_WALK_LEFT);
		break;
	case Client::Queen_MoggothWalk::WALK_RIGHT:
		SetAnimIndex(QUEEN_MOGGOTH_P1_WALK_RIGHT);
		break;
	default:
		break;
	}
}

Queen_MoggothWalk* Queen_MoggothWalk::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	return new Queen_MoggothWalk(pDevice, pContext);
}

void Queen_MoggothWalk::Free()
{
	__super::Free();
}


