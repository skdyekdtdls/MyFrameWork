#include "..\Public\CrystalGolemAttack.h"
#include "GameInstance.h"
#include "CrystalGolem.h"

CrystalGolemAttack::CrystalGolemAttack(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: StateMachine<CrystalGolem, CRYSTAL_GOLEM_ANIM>(pDevice, pContext)
{
}

CrystalGolemAttack::CrystalGolemAttack(const CrystalGolemAttack& rhs)
	: StateMachine<CrystalGolem, CRYSTAL_GOLEM_ANIM>(rhs)
{
}

void CrystalGolemAttack::OnStateEnter()
{
	__super::OnStateEnter();

	m_iLogic = RandomIntFrom_A_To_B(1, 2);
	switch (m_iLogic)
	{
	case 0:
		SetAnimIndex(CRYSTAL_GOLEM_ATTACK_AREA, LOWER); // 양손으로 내려찍기
		break;
	case 1:
		SetAnimIndex(CRYSTAL_GOLEM_ATTACK_AREA_02, LOWER); // 한손으로 내려찍기
		break;
	case 2:
		SetAnimIndex(CRYSTAL_GOLEM_ATTACK_AREA, LOWER);
		break;
	}

	m_iOrder = 0;
}

void CrystalGolemAttack::OnStateTick(_double TimeDelta)
{
	__super::OnStateTick(TimeDelta);

	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);
	CModel* pModel = static_cast<CModel*>(m_pOwner->Get_Component(L"Com_Model"));
	CTransform* pTransform = static_cast<CTransform*>(m_pOwner->Get_Component(L"Com_Transform"));

	switch (m_iLogic)
	{
	case 0:
		if (pModel->IsCurAnimTimeAccGreaterThan(55.f) && 0 == m_iOrder)
		{
			SetAnimIndex(CRYSTAL_GOLEM_ATTACK_AREA_02, LOWER); 
			m_iOrder = 1;
		}
		else if (pModel->IsAnimationFinished() && 1 == m_iOrder)
		{
			SetAnimIndex(CRYSTAL_GOLEM_ATTACK_RANGE, LOWER);
			m_iOrder = 2;
		}
		else if (pModel->IsAnimationFinished() && 2 == m_iOrder)
		{
			TransitionTo(L"CrystalGolemRun");
		}
		break;
	case 1:
		if (pModel->IsAnimationFinished())
		{
			TransitionTo(L"CrystalGolemRun");
		}
		break;
	case 2:
		if (pModel->IsAnimationFinished())
		{
			TransitionTo(L"CrystalGolemRun");
		}
		break;
	}

	Safe_Release(pGameInstance);
}

void CrystalGolemAttack::OnStateExit()
{
	__super::OnStateExit();
}

void CrystalGolemAttack::OnCollision(CCollider::COLLISION_INFO tCollisionInfo, _double TimeDelta)
{
	// 플레이어랑 레이랑 충돌하면 공격상태로 전환
	if (tCollisionInfo.pMyCollider == m_pOwner->GetComponent<Raycast>() &&
		tCollisionInfo.pOtherGameObject->GetInfo().wstrKey == TEXT("Prototype_GameObject_Clint"))
	{
		TransitionTo(L"CrystalGolemAttack");
	}
}

StateMachine<CrystalGolem, CRYSTAL_GOLEM_ANIM>* CrystalGolemAttack::Clone(void* pArg)
{
	__super::Initialize(pArg);
	return new CrystalGolemAttack(*this);
}

CrystalGolemAttack* CrystalGolemAttack::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	return new CrystalGolemAttack(pDevice, pContext);
}

void CrystalGolemAttack::Free()
{
	__super::Free();
}