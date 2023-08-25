#include "..\Public\CrystalGolemAppear.h"
#include "GameInstance.h"
#include "CrystalGolem.h"

CrystalGolemAppear::CrystalGolemAppear(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: StateMachine<CrystalGolem, CRYSTAL_GOLEM_ANIM>(pDevice, pContext)
{
}

CrystalGolemAppear::CrystalGolemAppear(const CrystalGolemAppear& rhs)
	: StateMachine<CrystalGolem, CRYSTAL_GOLEM_ANIM>(rhs)
{
}

void CrystalGolemAppear::OnStateEnter()
{
	__super::OnStateEnter();

	SetAnimIndex(CRYSTAL_GOLEM_APPEAR, LOWER);
}

void CrystalGolemAppear::OnStateTick(_double TimeDelta)
{
	__super::OnStateTick(TimeDelta);

	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);
	CModel* pModel = static_cast<CModel*>(m_pOwner->Get_Component(L"Com_Model"));
	CTransform* pTransform = static_cast<CTransform*>(m_pOwner->Get_Component(L"Com_Transform"));

	if (pModel->IsAnimationFinished())
	{
		TransitionTo(L"CrystalGolemRun");
	}
		
	Safe_Release(pGameInstance);
}

void CrystalGolemAppear::OnStateExit()
{
	__super::OnStateExit();
}

void CrystalGolemAppear::OnCollision(CCollider::COLLISION_INFO tCollisionInfo, _double TimeDelta)
{

}

StateMachine<CrystalGolem, CRYSTAL_GOLEM_ANIM>* CrystalGolemAppear::Clone(void* pArg)
{
	__super::Initialize(pArg);
	return new CrystalGolemAppear(*this);
}

CrystalGolemAppear* CrystalGolemAppear::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	return new CrystalGolemAppear(pDevice, pContext);
}

void CrystalGolemAppear::Free()
{
	__super::Free();
}