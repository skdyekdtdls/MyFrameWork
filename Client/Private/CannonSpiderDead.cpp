#include "..\Public\CannonSpiderDead.h"
#include "GameInstance.h"
#include "CannonSpider.h"

CannonSpiderDead::CannonSpiderDead(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: StateMachine<CannonSpider, CANNON_SPIDER_ANIM>(pDevice, pContext)
{
}

CannonSpiderDead::CannonSpiderDead(const CannonSpiderDead& rhs)
	: StateMachine<CannonSpider, CANNON_SPIDER_ANIM>(rhs)
{
}

void CannonSpiderDead::OnStateEnter()
{
	__super::OnStateEnter();

	SetAnimIndex(CANNON_SPIDER_STUNNED, LOWER);
}

void CannonSpiderDead::OnStateTick(_double TimeDelta)
{
	__super::OnStateTick(TimeDelta);

	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);
	CModel* pModel = static_cast<CModel*>(m_pOwner->Get_Component(L"Com_Model"));

	if (pModel->IsAnimationFinished())
		m_pOwner->SetDead();

	Safe_Release(pGameInstance);
}

void CannonSpiderDead::OnStateExit()
{
	__super::OnStateExit();
}

StateMachine<CannonSpider, CANNON_SPIDER_ANIM>* CannonSpiderDead::Clone(void* pArg)
{
	__super::Initialize(pArg);
	return new CannonSpiderDead(*this);
}

CannonSpiderDead* CannonSpiderDead::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	return new CannonSpiderDead(pDevice, pContext);
}

void CannonSpiderDead::Free()
{
	__super::Free();
}

