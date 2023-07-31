#include "..\Public\SpiderRun.h"
#include "GameInstance.h"
#include "Spider.h"

SpiderRun::SpiderRun(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: StateMachine<Spider, SPIDER_ANIM>(pDevice, pContext)
{
}

SpiderRun::SpiderRun(const SpiderRun& rhs)
	: StateMachine<Spider, SPIDER_ANIM>(rhs)
{
}

void SpiderRun::OnStateEnter()
{
	__super::OnStateEnter();

	SetAnimIndex(SPIDER_WALK, LOWER);
}

void SpiderRun::OnStateTick(_double TimeDelta)
{
	__super::OnStateTick(TimeDelta);

	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);
	CModel* pModel = static_cast<CModel*>(m_pOwner->Get_Component(L"Com_Model"));
	CTransform* pTransform = static_cast<CTransform*>(m_pOwner->Get_Component(L"Com_Transform"));

	Raycast* pRaycast = static_cast<Raycast*>(m_pOwner->Get_Component(L"Com_RayDetect"));
	CCollider* pCollider = static_cast<CCollider*>(m_pOwner->Get_Component(L"Com_BodyColl"));

	_vector vClintPos = Facade->GetClintPosition();
	_vector vMyPos = pTransform->Get_State(CTransform::STATE_POSITION);
	_vector vTargetVector = vClintPos - vMyPos;


	pTransform->Chase(vClintPos, TimeDelta, 0.5f);
	pTransform->Turn(WorldAxisY(), vTargetVector, TimeDelta);

	// 절두체 안에 있으면 콜라이더 그룹 추가.
	if (pGameInstance->isIn_WorldSpace(pTransform->Get_State(CTransform::STATE_POSITION), 1.f))
	{
		pRaycast->Add_ColliderGroup(COLL_GROUP::MONSTER_DETECT);
	}

	Safe_Release(pGameInstance);
}

void SpiderRun::OnStateExit()
{
	__super::OnStateExit();
}

void SpiderRun::OnCollision(CCollider::COLLISION_INFO tCollisionInfo, _double TimeDelta)
{
	// 플레이어랑 레이랑 충돌하면 공격상태로 전환
	if (tCollisionInfo.pMyCollider == m_pOwner->GetComponent<Raycast>() &&
		tCollisionInfo.pOtherGameObject->GetInfo().wstrKey == TEXT("Prototype_GameObject_Clint"))
	{
		TransitionTo(L"SpiderAttack");
	}
}

StateMachine<Spider, SPIDER_ANIM>* SpiderRun::Clone(void* pArg)
{
	__super::Initialize(pArg);
	return new SpiderRun(*this);
}

SpiderRun* SpiderRun::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	return new SpiderRun(pDevice, pContext);
}

void SpiderRun::Free()
{
	__super::Free();
}


