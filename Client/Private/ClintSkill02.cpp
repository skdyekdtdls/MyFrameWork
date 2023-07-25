#include "..\Public\ClintSkill02.h"
#include "GameInstance.h"
#include "Clint.h"

ClintSkill02::ClintSkill02(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: StateMachine<Clint, CLINT_ANIM>(pDevice, pContext)
{
}

ClintSkill02::ClintSkill02(const ClintSkill02& rhs)
	: StateMachine<Clint, CLINT_ANIM>(rhs)
{
}

void ClintSkill02::OnStateEnter()
{
	__super::OnStateEnter();

	SetAnimIndex(CLINT_SKILL_02, LOWER);
}

void ClintSkill02::OnStateTick(_double TimeDelta)
{
	__super::OnStateTick(TimeDelta);

	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);
	CModel* pModel = static_cast<CModel*>(m_pOwner->Get_Component(L"Com_Model"));
	CTransform* pTransform = static_cast<CTransform*>(m_pOwner->Get_Component(L"Com_Transform"));

	Safe_Release(pGameInstance);
}

void ClintSkill02::OnStateExit()
{
	__super::OnStateExit();
}

StateMachine<Clint, CLINT_ANIM>* ClintSkill02::Clone(void* pArg)
{
	__super::Initialize(pArg);
	return new ClintSkill02(*this);
}

ClintSkill02* ClintSkill02::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	return new ClintSkill02(pDevice, pContext);
}

void ClintSkill02::Free()
{
	__super::Free();
}