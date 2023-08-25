#include "..\Public\CrystalGolemRun.h"
#include "GameInstance.h"
#include "CrystalGolem.h"

CrystalGolemRun::CrystalGolemRun(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: StateMachine<CrystalGolem, CRYSTAL_GOLEM_ANIM>(pDevice, pContext)
{
}

CrystalGolemRun::CrystalGolemRun(const CrystalGolemRun& rhs)
	: StateMachine<CrystalGolem, CRYSTAL_GOLEM_ANIM>(rhs)
{
}

void CrystalGolemRun::OnStateEnter()
{
	__super::OnStateEnter();

	SetAnimIndex(CRYSTAL_GOLEM_WALK, LOWER);
	m_TimeAcc = { 0.0 };
}

void CrystalGolemRun::OnStateTick(_double TimeDelta)
{
	__super::OnStateTick(TimeDelta);

	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);
	CModel* pModel = static_cast<CModel*>(m_pOwner->Get_Component(L"Com_Model"));
	CTransform* pTransform = static_cast<CTransform*>(m_pOwner->Get_Component(L"Com_Transform"));
	CCollider* pCollider = static_cast<CColliderSphere*>(m_pOwner->Get_Component(L"Com_CollDetect"));

	pCollider->Add_ColliderGroup(COLL_GROUP::MONSTER_DETECT);

	if (false == pTransform->isFront(Single->GetClintPosition()))
	{
		m_TimeAcc += TimeDelta;
		if (m_TimeAcc >= 3.f)
			TransitionTo(L"CrystalGolemTurn180");
	}
	else
		m_TimeAcc = { 0.0 };

	CNavigation* pNavigation = static_cast<CNavigation*>(m_pOwner->Get_Component(L"Com_Navigation"));
	_vector vClintPos = Single->GetClintPosition();
	_vector vMyPos = pTransform->Get_State(CTransform::STATE_POSITION);
	_vector vTargetVector = vClintPos - vMyPos;

	pTransform->Go_Straight(TimeDelta, pNavigation);
	pTransform->Turn(WorldAxisY(), vTargetVector, TimeDelta);

	Safe_Release(pGameInstance);
}

void CrystalGolemRun::OnStateExit()
{
	__super::OnStateExit();
}

void CrystalGolemRun::OnCollision(CCollider::COLLISION_INFO tCollisionInfo, _double TimeDelta)
{
	// 플레이어랑 레이랑 충돌하면 공격상태로 전환
	if (tCollisionInfo.pMyCollider->GetComponentName() == L"Com_CollDetect" &&
		tCollisionInfo.pOtherGameObject->GetInfo().wstrKey == TEXT("Prototype_GameObject_Clint"))
	{
		TransitionTo(L"CrystalGolemAttack");
	}
}

StateMachine<CrystalGolem, CRYSTAL_GOLEM_ANIM>* CrystalGolemRun::Clone(void* pArg)
{
	__super::Initialize(pArg);
	return new CrystalGolemRun(*this);
}

CrystalGolemRun* CrystalGolemRun::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	return new CrystalGolemRun(pDevice, pContext);
}

void CrystalGolemRun::Free()
{
	__super::Free();
}


