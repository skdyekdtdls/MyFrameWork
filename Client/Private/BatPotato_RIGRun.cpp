#include "..\Public\BatPotato_RIGRun.h"
#include "GameInstance.h"
#include "BatPotato_RIG.h"
#include "Raycast.h"
BatPotato_RIGRun::BatPotato_RIGRun(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: StateMachine<BatPotato_RIG, BATPOTATO_RIG_ANIM>(pDevice, pContext)
{
}

BatPotato_RIGRun::BatPotato_RIGRun(const BatPotato_RIGRun& rhs)
	: StateMachine<BatPotato_RIG, BATPOTATO_RIG_ANIM>(rhs)
{
}

void BatPotato_RIGRun::OnStateEnter()
{
	__super::OnStateEnter();

	SetAnimIndex(BATPOTATO_RIG_RUN, LOWER);
}

void BatPotato_RIGRun::OnStateTick(_double TimeDelta)
{
	__super::OnStateTick(TimeDelta);

	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);
	CModel* pModel = static_cast<CModel*>(m_pOwner->Get_Component(L"Com_Model"));
	CTransform* pTransform = static_cast<CTransform*>(m_pOwner->Get_Component(L"Com_Transform"));
	Raycast* pRaycast = static_cast<Raycast*>(m_pOwner->Get_Component(L"Com_RayDetect"));
	CCollider* pCollider = static_cast<CCollider*>(m_pOwner->Get_Component(L"Com_BodyColl"));

	_vector vClintPos = Single->GetClintPosition();
	pTransform->Chase(vClintPos, TimeDelta, 6.f);
	pTransform->LookAt(vClintPos);

	// 절두체 안에 있으면 콜라이더 그룹 추가.
	pRaycast->Add_ColliderGroup(COLL_GROUP::MONSTER_DETECT);

	Safe_Release(pGameInstance);
}

void BatPotato_RIGRun::OnStateExit()
{
	__super::OnStateExit();
}

void BatPotato_RIGRun::OnCollision(CCollider::COLLISION_INFO tCollisionInfo, _double TimeDelta)
{
	if (tCollisionInfo.pMyCollider == m_pOwner->GetComponent<Raycast>() &&
		tCollisionInfo.pOtherGameObject->GetInfo().wstrKey == TEXT("Prototype_GameObject_Clint"))
	{
		TransitionTo(L"BatPotato_RIGAttack");
	}
}

StateMachine<BatPotato_RIG, BATPOTATO_RIG_ANIM>* BatPotato_RIGRun::Clone(void* pArg)
{
	__super::Initialize(pArg);
	return new BatPotato_RIGRun(*this);
}

BatPotato_RIGRun* BatPotato_RIGRun::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	return new BatPotato_RIGRun(pDevice, pContext);
}

void BatPotato_RIGRun::Free()
{
	__super::Free();
}