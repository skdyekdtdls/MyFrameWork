#include "..\Public\CannonSpiderIdle.h"
#include "GameInstance.h"
#include "CannonSpider.h"
#include "Collider.h"
CannonSpiderIdle::CannonSpiderIdle(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: StateMachine<CannonSpider, CANNON_SPIDER_ANIM>(pDevice, pContext)
{
}

CannonSpiderIdle::CannonSpiderIdle(const CannonSpiderIdle& rhs)
	: StateMachine<CannonSpider, CANNON_SPIDER_ANIM>(rhs)
{
}

void CannonSpiderIdle::OnStateEnter()
{
	__super::OnStateEnter();

	SetAnimIndex(CANNON_SPIDER_IDLE, LOWER);
	m_TimeAcc = { 0.0 };
}

void CannonSpiderIdle::OnStateTick(_double TimeDelta)
{
	__super::OnStateTick(TimeDelta);

	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);
	CModel* pModel = static_cast<CModel*>(m_pOwner->Get_Component(L"Com_Model"));
	CTransform* pTransform = static_cast<CTransform*>(m_pOwner->Get_Component(L"Com_Transform"));

	m_TimeAcc += TimeDelta;
	if (m_TimeAcc >= 1.f)
		TransitionTo(L"CannonSpiderRun");

	Safe_Release(pGameInstance);
}

void CannonSpiderIdle::OnStateExit()
{
	__super::OnStateExit();
}

void CannonSpiderIdle::OnCollision(CCollider::COLLISION_INFO tCollisionInfo, _double TimeDelta)
{
}

StateMachine<CannonSpider, CANNON_SPIDER_ANIM>* CannonSpiderIdle::Clone(void* pArg)
{
	__super::Initialize(pArg);
	return new CannonSpiderIdle(*this);
}

CannonSpiderIdle* CannonSpiderIdle::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	return new CannonSpiderIdle(pDevice, pContext);
}

void CannonSpiderIdle::Free()
{
	__super::Free();
}