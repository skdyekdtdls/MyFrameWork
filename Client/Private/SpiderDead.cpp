#include "..\Public\SpiderDead.h"
#include "GameInstance.h"
#include "Spider.h"

SpiderDead::SpiderDead(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: StateMachine<Spider, SPIDER_ANIM>(pDevice, pContext)
{
}

SpiderDead::SpiderDead(const SpiderDead& rhs)
	: StateMachine<Spider, SPIDER_ANIM>(rhs)
{
}

void SpiderDead::OnStateEnter()
{
	__super::OnStateEnter();

	SetAnimIndex(SPIDER_STUNNED, LOWER);
}

void SpiderDead::OnStateTick(_double TimeDelta)
{
	__super::OnStateTick(TimeDelta);

	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);
	CModel* pModel = static_cast<CModel*>(m_pOwner->Get_Component(L"Com_Model"));
	CTransform* pTransform = static_cast<CTransform*>(m_pOwner->Get_Component(L"Com_Transform"));

	if (pModel->IsAnimationFinished())
	{
		Single->AddExp(11.f);
		m_pOwner->SetDead();
		ObjectPool<Spider>::GetInstance()->PushPool(m_pOwner);
	}

	Safe_Release(pGameInstance);
}

void SpiderDead::OnStateExit()
{
	__super::OnStateExit();
}

void SpiderDead::OnCollision(CCollider::COLLISION_INFO tCollisionInfo, _double TimeDelta)
{

}

StateMachine<Spider, SPIDER_ANIM>* SpiderDead::Clone(void* pArg)
{
	__super::Initialize(pArg);
	return new SpiderDead(*this);
}

SpiderDead* SpiderDead::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	return new SpiderDead(pDevice, pContext);
}

void SpiderDead::Free()
{
	__super::Free();
}


