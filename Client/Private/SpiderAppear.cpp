#include "..\Public\SpiderAppear.h"
#include "GameInstance.h"
#include "Spider.h"

SpiderAppear::SpiderAppear(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: StateMachine<Spider, SPIDER_ANIM>(pDevice, pContext)
{
}

SpiderAppear::SpiderAppear(const SpiderAppear& rhs)
	: StateMachine<Spider, SPIDER_ANIM>(rhs)
{
}

void SpiderAppear::OnStateEnter()
{
	__super::OnStateEnter();

	SetAnimIndex(SPIDER_APPEAR, LOWER);
}

void SpiderAppear::OnStateTick(_double TimeDelta)
{
	__super::OnStateTick(TimeDelta);

	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);
	CModel* pModel = static_cast<CModel*>(m_pOwner->Get_Component(L"Com_Model"));
	CTransform* pTransform = static_cast<CTransform*>(m_pOwner->Get_Component(L"Com_Transform"));

	if (pModel->IsAnimationFinished())
		TransitionTo(L"SpiderRun");

	Safe_Release(pGameInstance);
}

void SpiderAppear::OnStateExit()
{
	__super::OnStateExit();
}

void SpiderAppear::OnCollision(CCollider::COLLISION_INFO tCollisionInfo, _double TimeDelta)
{

}

StateMachine<Spider, SPIDER_ANIM>* SpiderAppear::Clone(void* pArg)
{
	__super::Initialize(pArg);
	return new SpiderAppear(*this);
}

SpiderAppear* SpiderAppear::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	return new SpiderAppear(pDevice, pContext);
}

void SpiderAppear::Free()
{
	__super::Free();
}


