#include "..\Public\Queen_MoggothAppear.h"
#include "GameInstance.h"
#include "Queen_Moggoth.h"

Queen_MoggothAppear::Queen_MoggothAppear(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: StateMachine<Queen_Moggoth, QUEEN_MOGGOTH_ANIM>(pDevice, pContext)
{
}

Queen_MoggothAppear::Queen_MoggothAppear(const Queen_MoggothAppear& rhs)
	: StateMachine<Queen_Moggoth, QUEEN_MOGGOTH_ANIM>(rhs)
{
}

void Queen_MoggothAppear::OnStateEnter()
{
	__super::OnStateEnter();

	SetAnimIndex(QUEEN_MOGGOTH_APPEAR, LOWER);
}

void Queen_MoggothAppear::OnStateTick(_double TimeDelta)
{
	__super::OnStateTick(TimeDelta);

	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);
	CModel* pModel = static_cast<CModel*>(m_pOwner->Get_Component(L"Com_Model"));
	CTransform* pTransform = static_cast<CTransform*>(m_pOwner->Get_Component(L"Com_Transform"));

	if (pModel->IsAnimationFinished())
		TransitionTo(L"Queen_MoggothIdle");

	Safe_Release(pGameInstance);
}

void Queen_MoggothAppear::OnStateExit()
{
	__super::OnStateExit();
}

void Queen_MoggothAppear::OnCollision(CCollider::COLLISION_INFO tCollisionInfo, _double TimeDelta)
{

}

StateMachine<Queen_Moggoth, QUEEN_MOGGOTH_ANIM>* Queen_MoggothAppear::Clone(void* pArg)
{
	__super::Initialize(pArg);
	return new Queen_MoggothAppear(*this);
}

Queen_MoggothAppear* Queen_MoggothAppear::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	return new Queen_MoggothAppear(pDevice, pContext);
}

void Queen_MoggothAppear::Free()
{
	__super::Free();
}


