#include "..\Public\CrystalGolemTurn180.h"
#include "GameInstance.h"
#include "CrystalGolem.h"

CrystalGolemTurn180::CrystalGolemTurn180(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: StateMachine<CrystalGolem, CRYSTAL_GOLEM_ANIM>(pDevice, pContext)
{
}

CrystalGolemTurn180::CrystalGolemTurn180(const CrystalGolemTurn180& rhs)
	: StateMachine<CrystalGolem, CRYSTAL_GOLEM_ANIM>(rhs)
{
}

void CrystalGolemTurn180::OnStateEnter()
{
	__super::OnStateEnter();

	SetAnimIndex(CRYSTAL_GOLEM_TURN180, LOWER);
}

void CrystalGolemTurn180::OnStateTick(_double TimeDelta)
{
	__super::OnStateTick(TimeDelta);

	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);
	CModel* pModel = static_cast<CModel*>(m_pOwner->Get_Component(L"Com_Model"));
	CTransform* pTransform = static_cast<CTransform*>(m_pOwner->Get_Component(L"Com_Transform"));

	if (pModel->IsAnimationFinished())
	{
		TransitionTo(L"CrystalGolemIdle");
	}

	Safe_Release(pGameInstance);
}

void CrystalGolemTurn180::OnStateExit()
{
	__super::OnStateExit();
	CTransform* pTransform = static_cast<CTransform*>(m_pOwner->Get_Component(L"Com_Transform"));
	pTransform->RotationBack();
}

void CrystalGolemTurn180::OnCollision(CCollider::COLLISION_INFO tCollisionInfo, _double TimeDelta)
{

}

StateMachine<CrystalGolem, CRYSTAL_GOLEM_ANIM>* CrystalGolemTurn180::Clone(void* pArg)
{
	__super::Initialize(pArg);
	return new CrystalGolemTurn180(*this);
}

CrystalGolemTurn180* CrystalGolemTurn180::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	return new CrystalGolemTurn180(pDevice, pContext);
}

void CrystalGolemTurn180::Free()
{
	__super::Free();
}