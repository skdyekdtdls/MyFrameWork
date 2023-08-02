#include "..\Public\Alien_prawnDead.h"
#include "GameInstance.h"
#include "Alien_prawn.h"

Alien_prawnDead::Alien_prawnDead(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: StateMachine<Alien_prawn, ALIEN_PRAWN_ANIM>(pDevice, pContext)
{
}

Alien_prawnDead::Alien_prawnDead(const Alien_prawnDead& rhs)
	: StateMachine<Alien_prawn, ALIEN_PRAWN_ANIM>(rhs)
{
}

void Alien_prawnDead::OnStateEnter()
{
	__super::OnStateEnter();

	SetAnimIndex(ALIEN_PRAWN_HIT_LAUNCH_FALLING, LOWER);
}

void Alien_prawnDead::OnStateTick(_double TimeDelta)
{
	__super::OnStateTick(TimeDelta);

	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);
	CModel* pModel = static_cast<CModel*>(m_pOwner->Get_Component(L"Com_Model"));
	CTransform* pTransform = static_cast<CTransform*>(m_pOwner->Get_Component(L"Com_Transform"));

	if (pModel->IsAnimationFinished())
	{
		Single->AddExp(10.f);
		m_pOwner->SetDead();
	}

	Safe_Release(pGameInstance);
}

void Alien_prawnDead::OnStateExit()
{
	__super::OnStateExit();
}

StateMachine<Alien_prawn, ALIEN_PRAWN_ANIM>* Alien_prawnDead::Clone(void* pArg)
{
	__super::Initialize(pArg);
	return new Alien_prawnDead(*this);
}

Alien_prawnDead* Alien_prawnDead::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	return new Alien_prawnDead(pDevice, pContext);
}

void Alien_prawnDead::Free()
{
	__super::Free();
}
