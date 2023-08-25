#include "..\Public\Queen_MoggothAttackP2.h"
#include "GameInstance.h"
#include "Queen_Moggoth.h"

Queen_MoggothAttackP2::Queen_MoggothAttackP2(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: StateMachine<Queen_Moggoth, QUEEN_MOGGOTH_ANIM>(pDevice, pContext)
{
}

Queen_MoggothAttackP2::Queen_MoggothAttackP2(const Queen_MoggothAttackP2& rhs)
	: StateMachine<Queen_Moggoth, QUEEN_MOGGOTH_ANIM>(rhs)
{
}

void Queen_MoggothAttackP2::OnStateEnter()
{
	__super::OnStateEnter();

	SetAnimIndex(QUEEN_MOGGOTH_ANIM_END, LOWER);

	// 중복패턴 막음.
	while (m_ePattern == m_ePrevPattern)
		m_ePattern = static_cast<PATTERN>(RandomIntFrom_A_To_B(ATTACK01, PATTERN_END - 2));
	m_ePrevPattern = m_ePattern;

	// 값들 초기화
	SetAnimIndexByPattern();
	m_iLogic = 0;
	m_pModel = static_cast<CModel*>(m_pOwner->Get_Component(L"Com_Model"));
	m_pTransform = static_cast<CTransform*>(m_pOwner->Get_Component(L"Com_Transform"));
	m_pNavigation = static_cast<CNavigation*>(m_pOwner->Get_Component(L"Com_Navigation"));
	m_TimeAcc = { 0.0 };
	m_TimeAccShot = { 0.0 };
}

void Queen_MoggothAttackP2::OnStateTick(_double TimeDelta)
{
	__super::OnStateTick(TimeDelta);

	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);
	CModel* pModel = static_cast<CModel*>(m_pOwner->Get_Component(L"Com_Model"));
	CTransform* pTransform = static_cast<CTransform*>(m_pOwner->Get_Component(L"Com_Transform"));

	switch (m_ePattern)
	{
	case Client::Queen_MoggothAttackP2::ATTACK01:
		Attack01(TimeDelta);
		break;
	case Client::Queen_MoggothAttackP2::ATTACK02:
		Attack02(TimeDelta);
		break;
	case Client::Queen_MoggothAttackP2::ATTACK03:
		Attack03(TimeDelta);
		break;
	case Client::Queen_MoggothAttackP2::ATTACK04:
		Attack04(TimeDelta);
		break;
	}

	Safe_Release(pGameInstance);
}

void Queen_MoggothAttackP2::OnStateExit()
{
	__super::OnStateExit();
}

void Queen_MoggothAttackP2::OnCollision(CCollider::COLLISION_INFO tCollisionInfo, _double TimeDelta)
{

}

StateMachine<Queen_Moggoth, QUEEN_MOGGOTH_ANIM>* Queen_MoggothAttackP2::Clone(void* pArg)
{
	__super::Initialize(pArg);
	return new Queen_MoggothAttackP2(*this);
}

void Queen_MoggothAttackP2::Attack01(const _double& TimeDelta)
{
	if (m_pModel->IsAnimationFinished())
		TransitionTo(L"Queen_MoggothWalkP2");
}

void Queen_MoggothAttackP2::Attack02(const _double& TimeDelta)
{
	switch (m_iLogic)
	{
	case 0:
		if(m_pModel->IsAnimationFinished())
		{
			SetAnimIndex(QUEEN_MOGGOTH_P2_ATTACK02_LOOP);
			++m_iLogic;
		}
		break;
	case 1:
		m_TimeAcc += TimeDelta;
		if (m_TimeAcc >= 5.0)
		{
			SetAnimIndex(QUEEN_MOGGOTH_P2_ATTACK02_OUT);
			++m_iLogic;
		}
		break;
	case 2:
		if (m_pModel->IsAnimationFinished())
		{
			TransitionTo(L"Queen_MoggothWalkP2");
		}
		break;
	}
}

void Queen_MoggothAttackP2::Attack03(const _double& TimeDelta)
{
	if (m_pModel->IsAnimationFinished())
		TransitionTo(L"Queen_MoggothWalkP2");
}

void Queen_MoggothAttackP2::Attack04(const _double& TimeDelta)
{
	switch (m_iLogic)
	{
	case 0:
		
		if (m_pModel->IsAnimationFinished())
		{
			SetAnimIndex(QUEEN_MOGGOTH_P2_ATTACK04_LOOP);
			++m_iLogic;
		}
		break;
	case 1:
		m_TimeAcc += TimeDelta;
		m_TimeAccShot += TimeDelta;
		_vector vClintPos = Single->GetClintPosition();
		_vector vMyPos = m_pTransform->Get_State(CTransform::STATE_POSITION);
		_vector vTargetVector = vClintPos - vMyPos;
		m_pTransform->Turn(WorldAxisY(), vTargetVector, TimeDelta);

		// 1초마다 발사
		if (m_TimeAccShot >= 1.0)
		{
			m_TimeAccShot = { 0.0 };
			SetAnimIndex(QUEEN_MOGGOTH_P2_ATTACK04_SHOT);
		}

		// 애니메이션 재생 제어
		if (m_pModel->IsCurAnimTimeAccGreaterThan(15.f))
		{
			SetAnimIndex(QUEEN_MOGGOTH_P2_ATTACK04_LOOP);
		}

		// 8초지나면 패턴 해제
		if (m_TimeAcc >= 8.f)
		{
			SetAnimIndex(QUEEN_MOGGOTH_P2_ATTACK04_OUT);
			++m_iLogic;
		}
		break;
	case 2:
		if (m_pModel->IsAnimationFinished())
		{
			SetAnimIndex(QUEEN_MOGGOTH_P2_ATTACK04_OUT);
			++m_iLogic;
		}
		break;
	case 3:
		if (m_pModel->IsAnimationFinished())
		{
			TransitionTo(L"Queen_MoggothWalkP2");
		}
		break;
	}
}

void Queen_MoggothAttackP2::SetAnimIndexByPattern()
{
	switch (m_ePattern)
	{
	case Client::Queen_MoggothAttackP2::ATTACK01:
		SetAnimIndex(QUEEN_MOGGOTH_P2_ATTACK01);
		break;
	case Client::Queen_MoggothAttackP2::ATTACK02:
		SetAnimIndex(QUEEN_MOGGOTH_P2_ATTACK02_IN);
		break;
	case Client::Queen_MoggothAttackP2::ATTACK03:
		SetAnimIndex(QUEEN_MOGGOTH_P2_ATTACK03);
		break;
	case Client::Queen_MoggothAttackP2::ATTACK04:
		SetAnimIndex(QUEEN_MOGGOTH_P2_ATTACK04_IN);
		break;
	}
}

Queen_MoggothAttackP2* Queen_MoggothAttackP2::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	return new Queen_MoggothAttackP2(pDevice, pContext);
}

void Queen_MoggothAttackP2::Free()
{
	__super::Free();
}