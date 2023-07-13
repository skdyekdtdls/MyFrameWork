#include "..\Public\Alien_prawnAttack.h"
#include "GameInstance.h"
#include "Alien_prawn.h"
#include "StateContext.h"
Alien_prawnAttack::Alien_prawnAttack(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: StateMachine<Alien_prawn, ALIEN_PRAWN_ANIM>(pDevice, pContext)
{
}

Alien_prawnAttack::Alien_prawnAttack(const Alien_prawnAttack& rhs)
	: StateMachine<Alien_prawn, ALIEN_PRAWN_ANIM>(rhs)
	, iAttackIndex(0)
{
}

void Alien_prawnAttack::OnStateEnter()
{
	__super::OnStateEnter();

	RandomAttack();
}

void Alien_prawnAttack::OnStateTick(_double TimeDelta)
{
	__super::OnStateTick(TimeDelta);

	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);
	CModel* pModel = static_cast<CModel*>(m_pOwner->Get_Component(L"Com_Model"));
	if (pModel->IsAnimationFinished())
	{
		m_pStateContext->TransitionTo(L"Alien_prawnIdle");
	}

	Safe_Release(pGameInstance);
}

void Alien_prawnAttack::OnStateExit()
{
	__super::OnStateExit();
}

StateMachine<Alien_prawn, ALIEN_PRAWN_ANIM>* Alien_prawnAttack::Clone(void* pArg)
{
	__super::Initialize(pArg);
	return new Alien_prawnAttack(*this);
}

void Alien_prawnAttack::RandomAttack()
{
	switch (iAttackIndex)
	{
	case 0:
		SetAnimIndex(ALIEN_PRAWN_ATTACK01, LOWER);
		++iAttackIndex;
		break;
	case 1:
		SetAnimIndex(ALIEN_PRAWN_ATTACK01_MID, LOWER);
		++iAttackIndex;
		break;
	case 2:
		SetAnimIndex(ALIEN_PRAWN_ATTACK02, LOWER);
		iAttackIndex = { 0 };
		break;
	default:
		SetAnimIndex(ALIEN_PRAWN_ATTACK01, LOWER);
		iAttackIndex = { 0 };
		break;
	}
}

Alien_prawnAttack* Alien_prawnAttack::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	return new Alien_prawnAttack(pDevice, pContext);
}

void Alien_prawnAttack::Free()
{
	__super::Free();
}