#include "..\Public\ClintIdle.h"
#include "GameInstance.h"
#include "Clint.h"

ClintIdle::ClintIdle(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, Clint* pClint)
	: ClintState(pDevice, pContext, pClint)
{
}

void ClintIdle::OnStateEnter()
{
	__super::OnStateEnter();
	SetAnimIndex(CLINT_ANIM_IDLE, UPPER);
	SetAnimIndex(CLINT_ANIM_IDLE, LOWER);
}

void ClintIdle::OnStateTick(_double TimeDelta)
{
	__super::OnStateTick(TimeDelta);

	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	if (pGameInstance->Get_DIKeyState(DIK_W) || pGameInstance->Get_DIKeyState(DIK_A) ||
		pGameInstance->Get_DIKeyState(DIK_S) || pGameInstance->Get_DIKeyState(DIK_D))
	{
		m_pOwner->TransitionTo(L"ClintRun");
	}
	else if (pGameInstance->Get_DIKeyState(DIK_SPACE))
	{
		m_pOwner->TransitionTo(L"ClintDash");
	}
	else if (pGameInstance->Get_DIMouseState(CInput_Device::DIMK_LBUTTON))
	{
		m_pOwner->TransitionTo(L"ClintShoot");
	}
	Safe_Release(pGameInstance);
}

void ClintIdle::OnStateExit()
{
	__super::OnStateExit();
}

ClintIdle* ClintIdle::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, Clint* pClint)
{
	return new ClintIdle(pDevice, pContext, pClint);
}

void ClintIdle::Free()
{
	__super::Free();
}
