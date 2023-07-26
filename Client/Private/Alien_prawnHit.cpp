#include "..\Public\Alien_prawnHit.h"
#include "GameInstance.h"
#include "Alien_prawn.h"

Alien_prawnHit::Alien_prawnHit(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: StateMachine<Alien_prawn, ALIEN_PRAWN_ANIM>(pDevice, pContext)
{
}

Alien_prawnHit::Alien_prawnHit(const Alien_prawnHit& rhs)
	: StateMachine<Alien_prawn, ALIEN_PRAWN_ANIM>(rhs)
{
}

void Alien_prawnHit::OnStateEnter()
{
	__super::OnStateEnter();
	_int iRand = rand() % 2;
	
	if(0 == iRand)
		SetAnimIndex(ALIEN_PRAWN_HIT, LOWER);
	else
		SetAnimIndex(ALIEN_PRAWN_HIT_02, LOWER);
}

void Alien_prawnHit::OnStateTick(_double TimeDelta)
{
	__super::OnStateTick(TimeDelta);

	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);
	CModel* pModel = static_cast<CModel*>(m_pOwner->Get_Component(L"Com_Model"));
	CTransform* pTransform = static_cast<CTransform*>(m_pOwner->Get_Component(L"Com_Transform"));

	if (pModel->IsAnimationFinished())
		TransitionTo(L"Alien_prawnRun");

	Safe_Release(pGameInstance);
}

void Alien_prawnHit::OnStateExit()
{
	__super::OnStateExit();
}

StateMachine<Alien_prawn, ALIEN_PRAWN_ANIM>* Alien_prawnHit::Clone(void* pArg)
{
	__super::Initialize(pArg);
	return new Alien_prawnHit(*this);
}

Alien_prawnHit* Alien_prawnHit::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	return new Alien_prawnHit(pDevice, pContext);
}

void Alien_prawnHit::Free()
{
	__super::Free();
}





