#include "..\Public\BatPotato_RIGDeath.h"
#include "GameInstance.h"
#include "BatPotato_RIG.h"

BatPotato_RIGDeath::BatPotato_RIGDeath(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: StateMachine<BatPotato_RIG, BATPOTATO_RIG_ANIM>(pDevice, pContext)
{
}

BatPotato_RIGDeath::BatPotato_RIGDeath(const BatPotato_RIGDeath& rhs)
	: StateMachine<BatPotato_RIG, BATPOTATO_RIG_ANIM>(rhs)
{
}

void BatPotato_RIGDeath::OnStateEnter()
{
	__super::OnStateEnter();

	SetAnimIndex(BATPOTATO_RIG_HIT_LAUNCH_LAND, LOWER);
}

void BatPotato_RIGDeath::OnStateTick(_double TimeDelta)
{
	__super::OnStateTick(TimeDelta);

	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);
	CModel* pModel = static_cast<CModel*>(m_pOwner->Get_Component(L"Com_Model"));
	CTransform* pTransform = static_cast<CTransform*>(m_pOwner->Get_Component(L"Com_Transform"));

	if (pModel->IsAnimationFinished())
	{
		Single->AddExp(13.f);
		m_pOwner->SetDead();
	}

	Safe_Release(pGameInstance);
}

void BatPotato_RIGDeath::OnStateExit()
{
	__super::OnStateExit();
}

StateMachine<BatPotato_RIG, BATPOTATO_RIG_ANIM>* BatPotato_RIGDeath::Clone(void* pArg)
{
	__super::Initialize(pArg);
	return new BatPotato_RIGDeath(*this);
}

BatPotato_RIGDeath* BatPotato_RIGDeath::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	return new BatPotato_RIGDeath(pDevice, pContext);
}

void BatPotato_RIGDeath::Free()
{
	__super::Free();
}