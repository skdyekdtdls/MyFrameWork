#include "..\Public\ClintState.h"

ClintState::ClintState(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, Clint* pClint)
	: CBase()
	, m_pDevice(pDevice)
	, m_pContext(pContext)
	, m_pOwner(pClint)
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

void ClintState::OnStateEnter()
{
}

void ClintState::OnStateTick(double TimeDelta)
{
}

void ClintState::OnStateExit()
{
}

void ClintState::SetAnimIndex(CLINT_ANIM eClintAnim, BODY eBody)
{
	static_cast<CModel*>(m_pOwner->Get_Component(L"Com_Model"))->Set_AnimByIndex(eClintAnim, eBody);
}

void ClintState::Free()
{
	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
}
