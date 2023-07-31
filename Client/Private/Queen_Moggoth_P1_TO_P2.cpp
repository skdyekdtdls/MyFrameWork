#include "..\Public\Queen_Moggoth_P1_TO_P2.h"
#include "GameInstance.h"
#include "Queen_Moggoth.h"

Queen_Moggoth_P1_TO_P2::Queen_Moggoth_P1_TO_P2(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: StateMachine<Queen_Moggoth, QUEEN_MOGGOTH_ANIM>(pDevice, pContext)
{
}

Queen_Moggoth_P1_TO_P2::Queen_Moggoth_P1_TO_P2(const Queen_Moggoth_P1_TO_P2& rhs)
	: StateMachine<Queen_Moggoth, QUEEN_MOGGOTH_ANIM>(rhs)
{
}

void Queen_Moggoth_P1_TO_P2::OnStateEnter()
{
	__super::OnStateEnter();

	SetAnimIndex(QUEEN_MOGGOTH_P1_TO_P2, LOWER);
}

void Queen_Moggoth_P1_TO_P2::OnStateTick(_double TimeDelta)
{
	__super::OnStateTick(TimeDelta);

	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);
	CModel* pModel = static_cast<CModel*>(m_pOwner->Get_Component(L"Com_Model"));

	if (pModel->IsAnimationFinished())
		TransitionTo(L"Queen_MoggothIdleP2");
	Safe_Release(pGameInstance);
}

void Queen_Moggoth_P1_TO_P2::OnStateExit()
{
	__super::OnStateExit();
}

void Queen_Moggoth_P1_TO_P2::OnCollision(CCollider::COLLISION_INFO tCollisionInfo, _double TimeDelta)
{

}

StateMachine<Queen_Moggoth, QUEEN_MOGGOTH_ANIM>* Queen_Moggoth_P1_TO_P2::Clone(void* pArg)
{
	__super::Initialize(pArg);
	return new Queen_Moggoth_P1_TO_P2(*this);
}

Queen_Moggoth_P1_TO_P2* Queen_Moggoth_P1_TO_P2::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	return new Queen_Moggoth_P1_TO_P2(pDevice, pContext);
}

void Queen_Moggoth_P1_TO_P2::Free()
{
	__super::Free();
}


