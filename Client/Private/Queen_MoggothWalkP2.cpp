#include "..\Public\Queen_MoggothWalkP2.h"
#include "GameInstance.h"
#include "Queen_Moggoth.h"
#include "SoundMgr.h"
Queen_MoggothWalkP2::Queen_MoggothWalkP2(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: StateMachine<Queen_Moggoth, QUEEN_MOGGOTH_ANIM>(pDevice, pContext)
{
}

Queen_MoggothWalkP2::Queen_MoggothWalkP2(const Queen_MoggothWalkP2& rhs)
	: StateMachine<Queen_Moggoth, QUEEN_MOGGOTH_ANIM>(rhs)
{
}

void Queen_MoggothWalkP2::OnStateEnter()
{
	__super::OnStateEnter();

	SetAnimIndex(QUEEN_MOGGOTH_P2_IDLE, LOWER);

	m_ePattern = static_cast<PATTERN>(RandomIntFrom_A_To_B(0, PATTERN_END - 1));
	SetAnimIndexByPattern();
	m_pModel = static_cast<CModel*>(m_pOwner->Get_Component(L"Com_Model"));
	m_pTransform = static_cast<CTransform*>(m_pOwner->Get_Component(L"Com_Transform"));
	m_pNavigation = static_cast<CNavigation*>(m_pOwner->Get_Component(L"Com_Navigation"));
	m_TimeAcc = { 0.0 };
	SoundMgr->StopSound(CHANNELID::QUEENMOGGOTH);
	SoundMgr->PlaySound(L"QueenMoggothRunP2.ogg", CHANNELID::QUEENMOGGOTH, 0.4f);
}

void Queen_MoggothWalkP2::OnStateTick(_double TimeDelta)
{
	__super::OnStateTick(TimeDelta);

	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);
	CModel* pModel = static_cast<CModel*>(m_pOwner->Get_Component(L"Com_Model"));
	CTransform* pTransform = static_cast<CTransform*>(m_pOwner->Get_Component(L"Com_Transform"));

	_vector vClintPos = Single->GetClintPosition();
	_vector vMyPos = m_pTransform->Get_State(CTransform::STATE_POSITION);
	_vector vTargetVector = vClintPos - vMyPos;
	m_pTransform->Turn(WorldAxisY(), vTargetVector, TimeDelta);
	m_TimeAcc += TimeDelta;
	if (m_TimeAcc >= 2.0)
	{
		TransitionTo(L"Queen_MoggothAttackP2");
	}

	switch (m_ePattern)
	{
	case Client::Queen_MoggothWalkP2::WALK_BACK:
		m_pTransform->Go_Backward(TimeDelta, m_pNavigation);
		break;
	case Client::Queen_MoggothWalkP2::WALK_BL:
		m_pTransform->Go_Backward(TimeDelta, m_pNavigation);
		m_pTransform->Go_Left(TimeDelta, m_pNavigation);
		break;
	case Client::Queen_MoggothWalkP2::WALK_BR:
		m_pTransform->Go_Backward(TimeDelta, m_pNavigation);
		m_pTransform->Go_Right(TimeDelta, m_pNavigation);
		break;
	case Client::Queen_MoggothWalkP2::WALK_FL:
		m_pTransform->Go_Straight(TimeDelta, m_pNavigation);
		m_pTransform->Go_Left(TimeDelta, m_pNavigation);
		break;
	case Client::Queen_MoggothWalkP2::WALK_FR:
		m_pTransform->Go_Straight(TimeDelta, m_pNavigation);
		m_pTransform->Go_Right(TimeDelta, m_pNavigation);
		break;
	case Client::Queen_MoggothWalkP2::WALK_FWD:
		m_pTransform->Go_Straight(TimeDelta, m_pNavigation);
		break;
	case Client::Queen_MoggothWalkP2::WALK_LEFT:
		m_pTransform->Go_Left(TimeDelta, m_pNavigation);
		break;
	case Client::Queen_MoggothWalkP2::WALK_RIGHT:
		m_pTransform->Go_Right(TimeDelta, m_pNavigation);
		break;
	}

	Safe_Release(pGameInstance);
}

void Queen_MoggothWalkP2::OnStateExit()
{
	SoundMgr->StopSound(CHANNELID::QUEENMOGGOTH);
	__super::OnStateExit();
}

void Queen_MoggothWalkP2::OnCollision(CCollider::COLLISION_INFO tCollisionInfo, _double TimeDelta)
{

}

StateMachine<Queen_Moggoth, QUEEN_MOGGOTH_ANIM>* Queen_MoggothWalkP2::Clone(void* pArg)
{
	__super::Initialize(pArg);
	return new Queen_MoggothWalkP2(*this);
}

void Queen_MoggothWalkP2::SetAnimIndexByPattern()
{
	switch (m_ePattern)
	{
	case Client::Queen_MoggothWalkP2::WALK_BACK:
		SetAnimIndex(QUEEN_MOGGOTH_P2_WALK_BACK);
		break;
	case Client::Queen_MoggothWalkP2::WALK_BL:
		SetAnimIndex(QUEEN_MOGGOTH_P2_WALK_BL);
		break;
	case Client::Queen_MoggothWalkP2::WALK_BR:
		SetAnimIndex(QUEEN_MOGGOTH_P2_WALK_BR);
		break;
	case Client::Queen_MoggothWalkP2::WALK_FL:
		SetAnimIndex(QUEEN_MOGGOTH_P2_WALK_FL);
		break;
	case Client::Queen_MoggothWalkP2::WALK_FR:
		SetAnimIndex(QUEEN_MOGGOTH_P2_WALK_FR);
		break;
	case Client::Queen_MoggothWalkP2::WALK_FWD:
		SetAnimIndex(QUEEN_MOGGOTH_P2_WALK_FWD);
		break;
	case Client::Queen_MoggothWalkP2::WALK_LEFT:
		SetAnimIndex(QUEEN_MOGGOTH_P2_WALK_LEFT);
		break;
	case Client::Queen_MoggothWalkP2::WALK_RIGHT:
		SetAnimIndex(QUEEN_MOGGOTH_P2_WALK_RIGHT);
		break;
	}
}

Queen_MoggothWalkP2* Queen_MoggothWalkP2::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	return new Queen_MoggothWalkP2(pDevice, pContext);
}

void Queen_MoggothWalkP2::Free()
{
	__super::Free();
}


