#include "..\Public\SpiderIdle.h"
#include "GameInstance.h"
#include "Spider.h"

SpiderIdle::SpiderIdle(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: StateMachine<Spider, SPIDER_ANIM>(pDevice, pContext)
{
}

SpiderIdle::SpiderIdle(const SpiderIdle& rhs)
	: StateMachine<Spider, SPIDER_ANIM>(rhs)
{
}

void SpiderIdle::OnStateEnter()
{
	__super::OnStateEnter();

	SetAnimIndex(SPIDER_IDLE, LOWER);
	m_TimeAcc = { 0.0 };
}

void SpiderIdle::OnStateTick(_double TimeDelta)
{
	__super::OnStateTick(TimeDelta);

	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);
	CModel* pModel = static_cast<CModel*>(m_pOwner->Get_Component(L"Com_Model"));
	CTransform* pTransform = static_cast<CTransform*>(m_pOwner->Get_Component(L"Com_Transform"));
	m_TimeAcc += TimeDelta;

	if (m_TimeAcc > 1.0)
		TransitionTo(L"SpiderRun");

	Safe_Release(pGameInstance);
}

void SpiderIdle::OnStateExit()
{
	__super::OnStateExit();
}

void SpiderIdle::OnCollision(CCollider::COLLISION_INFO tCollisionInfo, _double TimeDelta)
{

}

StateMachine<Spider, SPIDER_ANIM>* SpiderIdle::Clone(void* pArg)
{
	__super::Initialize(pArg);
	return new SpiderIdle(*this);
}

SpiderIdle* SpiderIdle::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	return new SpiderIdle(pDevice, pContext);
}

void SpiderIdle::Free()
{
	__super::Free();
}


