#include "..\Public\CannonSpiderRun.h"
#include "GameInstance.h"
#include "CannonSpider.h"

CannonSpiderRun::CannonSpiderRun(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: StateMachine<CannonSpider, CANNON_SPIDER_ANIM>(pDevice, pContext)
{
}

CannonSpiderRun::CannonSpiderRun(const CannonSpiderRun& rhs)
	: StateMachine<CannonSpider, CANNON_SPIDER_ANIM>(rhs)
{
}

void CannonSpiderRun::OnStateEnter()
{
	__super::OnStateEnter();

	SetAnimIndex(CANNON_SPIDER_RUN_F, LOWER);
}

void CannonSpiderRun::OnStateTick(_double TimeDelta)
{
	__super::OnStateTick(TimeDelta);

	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);
	CModel* pModel = static_cast<CModel*>(m_pOwner->Get_Component(L"Com_Model"));
	CTransform* pTransform = static_cast<CTransform*>(m_pOwner->Get_Component(L"Com_Transform"));
	Raycast* pRaycast = static_cast<Raycast*>(m_pOwner->Get_Component(L"Com_RayDetect"));
	CCollider* pCollider = static_cast<CCollider*>(m_pOwner->Get_Component(L"Com_BodyColl"));

	_vector vClintPos = Facade->GetClintPosition();
	pTransform->Chase(vClintPos, TimeDelta, 6.f);
	pTransform->LookAt(vClintPos);

	// 절두체 안에 있으면 콜라이더 그룹 추가.
	pRaycast->Add_ColliderGroup(COLL_GROUP::MONSTER_DETECT);

	Safe_Release(pGameInstance);
}

void CannonSpiderRun::OnStateExit()
{
	__super::OnStateExit();
}

void CannonSpiderRun::OnCollision(CCollider::COLLISION_INFO tCollisionInfo, _double TimeDelta)
{
	if (tCollisionInfo.pMyCollider == m_pOwner->GetComponent<Raycast>() &&
		tCollisionInfo.pOtherGameObject->GetInfo().wstrKey == TEXT("Prototype_GameObject_Clint"))
	{
		TransitionTo(L"CannonSpiderAttack");
	}
}

StateMachine<CannonSpider, CANNON_SPIDER_ANIM>* CannonSpiderRun::Clone(void* pArg)
{
	__super::Initialize(pArg);
	return new CannonSpiderRun(*this);
}

CannonSpiderRun* CannonSpiderRun::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	return new CannonSpiderRun(pDevice, pContext);
}

void CannonSpiderRun::Free()
{
	__super::Free();
}