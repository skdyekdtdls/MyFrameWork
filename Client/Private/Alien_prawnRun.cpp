#include "..\Public\Alien_prawnRun.h"
#include "GameInstance.h"
#include "Alien_prawn.h"
#include "StateContext.h"
Alien_prawnRun::Alien_prawnRun(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: StateMachine<Alien_prawn, ALIEN_PRAWN_ANIM>(pDevice, pContext)
{
}

Alien_prawnRun::Alien_prawnRun(const Alien_prawnRun& rhs)
	: StateMachine<Alien_prawn, ALIEN_PRAWN_ANIM>(rhs)
{
}

void Alien_prawnRun::OnStateEnter()
{
	__super::OnStateEnter();

	SetAnimIndex(ALIEN_PRAWN_RUN, LOWER);
}

void Alien_prawnRun::OnStateTick(_double TimeDelta)
{
	__super::OnStateTick(TimeDelta);

	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);
	CModel* pModel = static_cast<CModel*>(m_pOwner->Get_Component(L"Com_Model"));
	CTransform* pTransform = static_cast<CTransform*>(m_pOwner->Get_Component(L"Com_Transform"));
	_vector vClintPos = Facade->GetClintPosition();
	pTransform->Chase(vClintPos, TimeDelta, 0.5f);
	pTransform->LookAt(vClintPos);

	if (Facade->IntersectClintBodyColl(static_cast<CCollider*>(m_pOwner->Get_Component(L"Com_RayDetect"))))
	{
		m_pStateContext->TransitionTo(L"Alien_prawnAttack");
	}

	Safe_Release(pGameInstance);
}

void Alien_prawnRun::OnStateExit()
{
	__super::OnStateExit();
}

StateMachine<Alien_prawn, ALIEN_PRAWN_ANIM>* Alien_prawnRun::Clone(void* pArg)
{
	__super::Initialize(pArg);
	return new Alien_prawnRun(*this);
}

Alien_prawnRun* Alien_prawnRun::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	return new Alien_prawnRun(pDevice, pContext);
}

void Alien_prawnRun::Free()
{
	__super::Free();
}


