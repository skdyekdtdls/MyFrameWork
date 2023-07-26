#include "..\Public\Alien_prawnIdle.h"
#include "GameInstance.h"
#include "Alien_prawn.h"

Alien_prawnIdle::Alien_prawnIdle(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: StateMachine<Alien_prawn, ALIEN_PRAWN_ANIM>(pDevice, pContext)
{
}

Alien_prawnIdle::Alien_prawnIdle(const Alien_prawnIdle& rhs)
	: StateMachine<Alien_prawn, ALIEN_PRAWN_ANIM>(rhs)
	, m_TimeAcc(0.0)
{
}

void Alien_prawnIdle::OnStateEnter()
{
	__super::OnStateEnter();
	m_TimeAcc = { 0.0 };
	SetAnimIndex(ALIEN_PRAWN_IDLE, LOWER);
}

void Alien_prawnIdle::OnStateTick(_double TimeDelta)
{
	__super::OnStateTick(TimeDelta);

	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);
	CModel* pModel = static_cast<CModel*>(m_pOwner->Get_Component(L"Com_Model"));
	CTransform* pTransform = static_cast<CTransform*>(m_pOwner->Get_Component(L"Com_Transform"));
	Raycast* pRaycast = static_cast<Raycast*>(m_pOwner->Get_Component(L"Com_RayDetect"));
	CCollider* pCollider = static_cast<CCollider*>(m_pOwner->Get_Component(L"Com_BodyColl"));

	m_TimeAcc += TimeDelta;
	
	if (m_TimeAcc >= 2.0)
	{
		TransitionTo(L"Alien_prawnRun");
	}

	Safe_Release(pGameInstance);
}

void Alien_prawnIdle::OnStateExit()
{
	__super::OnStateExit();
}

void Alien_prawnIdle::OnCollision(CCollider::COLLISION_INFO tCollisionInfo, _double TimeDelta)
{

}

StateMachine<Alien_prawn, ALIEN_PRAWN_ANIM>* Alien_prawnIdle::Clone(void* pArg)
{
	__super::Initialize(pArg);
	return new Alien_prawnIdle(*this);
}

Alien_prawnIdle* Alien_prawnIdle::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	return new Alien_prawnIdle(pDevice, pContext);
}

void Alien_prawnIdle::Free()
{
	__super::Free();
}