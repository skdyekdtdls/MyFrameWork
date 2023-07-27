#include "..\Public\CannonSpiderAttack.h"
#include "GameInstance.h"
#include "CannonSpider.h"

CannonSpiderAttack::CannonSpiderAttack(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: StateMachine<CannonSpider, CANNON_SPIDER_ANIM>(pDevice, pContext)
{
}

CannonSpiderAttack::CannonSpiderAttack(const CannonSpiderAttack& rhs)
	: StateMachine<CannonSpider, CANNON_SPIDER_ANIM>(rhs)
{
}

void CannonSpiderAttack::OnStateEnter()
{
	__super::OnStateEnter();
	switch (RandomIntFrom_A_To_B(0, 1))
	{
	case 0:
		SetAnimIndex(CANNON_SPIDER_ATTACK01, LOWER);
		break;
	case 1:
		SetAnimIndex(CANNON_SPIDER_ATTACK01, LOWER);
		break;
	}
}

void CannonSpiderAttack::OnStateTick(_double TimeDelta)
{
	__super::OnStateTick(TimeDelta);

	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);
	CModel* pModel = static_cast<CModel*>(m_pOwner->Get_Component(L"Com_Model"));
	CTransform* pTransform = static_cast<CTransform*>(m_pOwner->Get_Component(L"Com_Transform"));

	if (pModel->IsAnimationFinished())
		TransitionTo(L"CannonSpiderRun");
	else
		pModel->RootMotion(TimeDelta, pTransform->Get_State(CTransform::STATE_LOOK));

	Safe_Release(pGameInstance);
}

void CannonSpiderAttack::OnStateExit()
{
	__super::OnStateExit();
}

void CannonSpiderAttack::OnCollision(CCollider::COLLISION_INFO tCollisionInfo, _double TimeDelta)
{

}

StateMachine<CannonSpider, CANNON_SPIDER_ANIM>* CannonSpiderAttack::Clone(void* pArg)
{
	__super::Initialize(pArg);
	return new CannonSpiderAttack(*this);
}

CannonSpiderAttack* CannonSpiderAttack::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	return new CannonSpiderAttack(pDevice, pContext);
}

void CannonSpiderAttack::Free()
{
	__super::Free();
}