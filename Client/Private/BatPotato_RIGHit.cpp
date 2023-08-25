#include "..\Public\BatPotato_RIGHit.h"
#include "GameInstance.h"
#include "BatPotato_RIG.h"

BatPotato_RIGHit::BatPotato_RIGHit(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: StateMachine<BatPotato_RIG, BATPOTATO_RIG_ANIM>(pDevice, pContext)
{
}

BatPotato_RIGHit::BatPotato_RIGHit(const BatPotato_RIGHit& rhs)
	: StateMachine<BatPotato_RIG, BATPOTATO_RIG_ANIM>(rhs)
{
}

void BatPotato_RIGHit::OnStateEnter()
{
	__super::OnStateEnter();

	SetAnimIndex(BATPOTATO_RIG_HIT, LOWER);
}

void BatPotato_RIGHit::OnStateTick(_double TimeDelta)
{
	__super::OnStateTick(TimeDelta);

	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);
	CModel* pModel = static_cast<CModel*>(m_pOwner->Get_Component(L"Com_Model"));

	if (pModel->IsAnimationFinished())
		TransitionTo(L"BatPotato_RIGRun");

	Safe_Release(pGameInstance);
}

void BatPotato_RIGHit::OnStateExit()
{
	__super::OnStateExit();
}

StateMachine<BatPotato_RIG, BATPOTATO_RIG_ANIM>* BatPotato_RIGHit::Clone(void* pArg)
{
	__super::Initialize(pArg);
	return new BatPotato_RIGHit(*this);
}

BatPotato_RIGHit* BatPotato_RIGHit::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	return new BatPotato_RIGHit(pDevice, pContext);
}

void BatPotato_RIGHit::Free()
{
	__super::Free();
}