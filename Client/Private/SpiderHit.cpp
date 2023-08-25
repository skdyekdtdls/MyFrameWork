#include "..\Public\SpiderHit.h"
#include "GameInstance.h"
#include "Spider.h"

SpiderHit::SpiderHit(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: StateMachine<Spider, SPIDER_ANIM>(pDevice, pContext)
{
}

SpiderHit::SpiderHit(const SpiderHit& rhs)
	: StateMachine<Spider, SPIDER_ANIM>(rhs)
{
}

void SpiderHit::OnStateEnter()
{
	__super::OnStateEnter();

	_uint iLogic = RandomIntFrom_A_To_B(0, 1);
	switch (iLogic)
	{
	case 0:
		SetAnimIndex(SPIDER_HIT, LOWER); 
		break;
	case 1:
		SetAnimIndex(SPIDER_HIT_B, LOWER);
	}
}

void SpiderHit::OnStateTick(_double TimeDelta)
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

void SpiderHit::OnStateExit()
{
	__super::OnStateExit();
}

void SpiderHit::OnCollision(CCollider::COLLISION_INFO tCollisionInfo, _double TimeDelta)
{

}

StateMachine<Spider, SPIDER_ANIM>* SpiderHit::Clone(void* pArg)
{
	__super::Initialize(pArg);
	return new SpiderHit(*this);
}

SpiderHit* SpiderHit::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	return new SpiderHit(pDevice, pContext);
}

void SpiderHit::Free()
{
	__super::Free();
}


