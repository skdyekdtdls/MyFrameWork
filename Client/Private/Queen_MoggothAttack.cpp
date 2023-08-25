#include "..\Public\Queen_MoggothAttack.h"
#include "GameInstance.h"
#include "Queen_Moggoth.h"
#include "HP.h"
#include "PlayerHP.h"

Queen_MoggothAttack::Queen_MoggothAttack(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: StateMachine<Queen_Moggoth, QUEEN_MOGGOTH_ANIM>(pDevice, pContext)
{
}

Queen_MoggothAttack::Queen_MoggothAttack(const Queen_MoggothAttack& rhs)
	: StateMachine<Queen_Moggoth, QUEEN_MOGGOTH_ANIM>(rhs)
{
}

void Queen_MoggothAttack::OnStateEnter()
{
	__super::OnStateEnter();

	// 중복패턴 막음.
	while (m_ePattern == m_ePrevPattern)
		m_ePattern = static_cast<PATTERN>(RandomIntFrom_A_To_B(ATTACK01, PATTERN_END - 1));
	m_ePrevPattern = m_ePattern;

	if (m_ePattern == ATTACK03)
	{
		m_ePattern = ATTACK04;
		m_ePrevPattern = m_ePattern;
	}

	// 값들 초기화
	SetAnimIndexByPattern();
	m_iLogic = 0;
	m_pModel = static_cast<CModel*>(m_pOwner->Get_Component(L"Com_Model"));
	m_pTransform = static_cast<CTransform*>(m_pOwner->Get_Component(L"Com_Transform"));
	m_pNavigation = static_cast<CNavigation*>(m_pOwner->Get_Component(L"Com_Navigation"));
	m_pAttack04Coll = static_cast<CColliderAABB*>(m_pOwner->Get_Component(L"Com_Attack04Coll"));

	m_TimeAcc = { 0.0 };
}

void Queen_MoggothAttack::OnStateTick(_double TimeDelta)
{
	__super::OnStateTick(TimeDelta);
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	switch (m_ePattern)
	{
	case Client::Queen_MoggothAttack::ATTACK01:
		Attack01(TimeDelta);
		break;
	case Client::Queen_MoggothAttack::ATTACK02:
		Attack02(TimeDelta);
		break;
	case Client::Queen_MoggothAttack::ATTACK03:
		Attack03(TimeDelta);
		break;
	case Client::Queen_MoggothAttack::ATTACK04:
		Attack04(TimeDelta);
		break;
	}

	Safe_Release(pGameInstance);
}

void Queen_MoggothAttack::OnStateExit()
{
	__super::OnStateExit();
}

void Queen_MoggothAttack::OnCollision(CCollider::COLLISION_INFO tCollisionInfo, _double TimeDelta)
{
	if (tCollisionInfo.MyCollName == L"Com_Attack04Coll"
		&& 0 == strcmp(tCollisionInfo.OtherGameObejctName.c_str(), "Clint1"))
	{
		//HP* pHP = dynamic_cast<HP*>(m_pOwner->Get_Component(L"Com_HP"));

		//if (pHP)
		//	pHP->TakeDamage(static_cast<_uint>(1.f));
		
		HP* pHealth = (tCollisionInfo.pOtherGameObject)->GetComponent<HP>();
		pHealth->TakeDamage(1);
	}
}

StateMachine<Queen_Moggoth, QUEEN_MOGGOTH_ANIM>* Queen_MoggothAttack::Clone(void* pArg)
{
	__super::Initialize(pArg);
	return new Queen_MoggothAttack(*this);
}

void Queen_MoggothAttack::Attack01(const _double& TimeDelta)
{
	if (m_pModel->IsAnimationFinished())
		TransitionTo(L"Queen_MoggothWalk");
}

void Queen_MoggothAttack::Attack02(const _double& TimeDelta)
{
	if (m_pModel->IsAnimationFinished())
		TransitionTo(L"Queen_MoggothWalk");
}

void Queen_MoggothAttack::Attack03(const _double& TimeDelta)
{
	if (m_pModel->IsAnimationFinished())
		TransitionTo(L"Queen_MoggothWalk");
}

void Queen_MoggothAttack::Attack04(const _double& TimeDelta)
{
	switch (m_iLogic)
	{
	case 0:
	{
		m_pModel->RootMotion(TimeDelta, m_pTransform->Get_State(CTransform::STATE_LOOK));
		if (m_pModel->IsCurAnimTimeAccLessThan(85.0)
			&& m_pModel->IsCurAnimTimeAccGreaterThan(50.0))
		{
			m_pAttack04Coll->Add_ColliderGroup(COLL_GROUP::MONSTER_BULLET);
			m_pAttack04Coll->Tick(m_pTransform->Get_WorldMatrix());
		}
		
		if (m_pModel->IsAnimationFinished())
		{
			SetAnimIndex(QUEEN_MOGGOTH_P1_ATTACK04_LOOP);
			++m_iLogic;
		}
	}
		break;
	case 1:
	{
		m_TimeAcc += TimeDelta;
		if (m_TimeAcc >= 7.0)
		{
			SetAnimIndex(QUEEN_MOGGOTH_P1_ATTACK04_OUT);
			++m_iLogic;
		}
	}
	case 2:
	{
		if (m_pModel->IsAnimationFinished())
		{
			TransitionTo(L"Queen_MoggothWalk");
		}
	}
		break;
	}
}

void Queen_MoggothAttack::SetAnimIndexByPattern()
{
	switch (m_ePattern)
	{
	case Client::Queen_MoggothAttack::ATTACK01:
		SetAnimIndex(QUEEN_MOGGOTH_P1_ATTACK01);
		break;
	case Client::Queen_MoggothAttack::ATTACK02:
		SetAnimIndex(QUEEN_MOGGOTH_P1_ATTACK02);
		break;
	case Client::Queen_MoggothAttack::ATTACK03:
		SetAnimIndex(QUEEN_MOGGOTH_P1_ATTACK03);
		break;
	case Client::Queen_MoggothAttack::ATTACK04:
		SetAnimIndex(QUEEN_MOGGOTH_P1_ATTACK04_IN);
		break;
	default:
		break;
	}
}

Queen_MoggothAttack* Queen_MoggothAttack::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	return new Queen_MoggothAttack(pDevice, pContext);
}

void Queen_MoggothAttack::Free()
{
	__super::Free();
}


