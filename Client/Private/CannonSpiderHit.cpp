#include "..\Public\CannonSpiderHit.h"
#include "GameInstance.h"
#include "CannonSpider.h"

CannonSpiderHit::CannonSpiderHit(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: StateMachine<CannonSpider, CANNON_SPIDER_ANIM>(pDevice, pContext)
{
}

CannonSpiderHit::CannonSpiderHit(const CannonSpiderHit& rhs)
	: StateMachine<CannonSpider, CANNON_SPIDER_ANIM>(rhs)
{
}

void CannonSpiderHit::OnStateEnter()
{
	__super::OnStateEnter();

	SetAnimIndex(CANNON_SPIDER_HIT, LOWER);
}

void CannonSpiderHit::OnStateTick(_double TimeDelta)
{
	__super::OnStateTick(TimeDelta);

	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);
	CModel* pModel = static_cast<CModel*>(m_pOwner->Get_Component(L"Com_Model"));
	CTransform* pTransform = static_cast<CTransform*>(m_pOwner->Get_Component(L"Com_Transform"));

	if (pModel->IsAnimationFinished())
		TransitionTo(L"CannonSpiderRun");

	Safe_Release(pGameInstance);
}

void CannonSpiderHit::OnStateExit()
{
	__super::OnStateExit();
}

void CannonSpiderHit::OnCollision(CCollider::COLLISION_INFO tCollisionInfo, _double TimeDelta)
{
}

StateMachine<CannonSpider, CANNON_SPIDER_ANIM>* CannonSpiderHit::Clone(void* pArg)
{
	__super::Initialize(pArg);
	return new CannonSpiderHit(*this);
}

CannonSpiderHit* CannonSpiderHit::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	return new CannonSpiderHit(pDevice, pContext);
}

void CannonSpiderHit::Free()
{
	__super::Free();
}