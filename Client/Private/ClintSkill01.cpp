#include "..\Public\ClintSkill01.h"
#include "GameInstance.h"
#include "Clint.h"

ClintSkill01::ClintSkill01(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: StateMachine<Clint, CLINT_ANIM>(pDevice, pContext)
{
}

ClintSkill01::ClintSkill01(const ClintSkill01& rhs)
	: StateMachine<Clint, CLINT_ANIM>(rhs)
{
}

void ClintSkill01::OnStateEnter()
{
	__super::OnStateEnter();

	SetAnimIndex(CLINT_SKILL_01, LOWER);
	SetAnimIndex(CLINT_SKILL_01, UPPER);
}

void ClintSkill01::OnStateTick(_double TimeDelta)
{
	__super::OnStateTick(TimeDelta);

	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);
	CModel* pModel = static_cast<CModel*>(m_pOwner->Get_Component(L"Com_Model"));
	CTransform* pTransform = static_cast<CTransform*>(m_pOwner->Get_Component(L"Com_Transform"));

	if (pModel->IsAnimationFinished())
	{
		TransitionTo(L"ClintIdle");
	}

	Safe_Release(pGameInstance);
}

void ClintSkill01::OnStateExit()
{
	__super::OnStateExit();
}

StateMachine<Clint, CLINT_ANIM>* ClintSkill01::Clone(void* pArg)
{
	__super::Initialize(pArg);
	return new ClintSkill01(*this);
}

ClintSkill01* ClintSkill01::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	return new ClintSkill01(pDevice, pContext);
}

void ClintSkill01::Free()
{
	__super::Free();
}