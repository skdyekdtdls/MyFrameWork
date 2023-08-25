#include "..\Public\SpiderAttack.h"
#include "GameInstance.h"
#include "Spider.h"

SpiderAttack::SpiderAttack(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: StateMachine<Spider, SPIDER_ANIM>(pDevice, pContext)
{
}

SpiderAttack::SpiderAttack(const SpiderAttack& rhs)
	: StateMachine<Spider, SPIDER_ANIM>(rhs)
{
}

void SpiderAttack::OnStateEnter()
{
	__super::OnStateEnter();

	_uint iLogic = RandomIntFrom_A_To_B(0, 2);
	switch (iLogic)
	{
	case 0:
		SetAnimIndex(SPIDER_ATTACK);
		break;
	case 1:
		SetAnimIndex(SPIDER_ATTACK_B);
		break;
	case 2:
		SetAnimIndex(SPIDER_ATTACK_MID);
		break;
	}
}

void SpiderAttack::OnStateTick(_double TimeDelta)
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

void SpiderAttack::OnStateExit()
{
	__super::OnStateExit();
}

void SpiderAttack::OnCollision(CCollider::COLLISION_INFO tCollisionInfo, _double TimeDelta)
{

}

StateMachine<Spider, SPIDER_ANIM>* SpiderAttack::Clone(void* pArg)
{
	__super::Initialize(pArg);
	return new SpiderAttack(*this);
}

SpiderAttack* SpiderAttack::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	return new SpiderAttack(pDevice, pContext);
}

void SpiderAttack::Free()
{
	__super::Free();
}


