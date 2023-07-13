#include "..\Public\ClintRun.h"
#include "GameInstance.h"
#include "Clint.h"

ClintRun::ClintRun(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: StateMachine<Clint, CLINT_ANIM>(pDevice, pContext)
{
}

ClintRun::ClintRun(const ClintRun& rhs)
	: StateMachine<Clint, CLINT_ANIM>(rhs)
{
}

void ClintRun::OnStateEnter()
{
	__super::OnStateEnter();
	SetAnimIndex(CLINT_ANIM_RUN, UPPER);
	SetAnimIndex(CLINT_ANIM_RUN, LOWER);
}

void ClintRun::OnStateTick(_double TimeDelta)
{
	__super::OnStateTick(TimeDelta);

	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);
	CModel* pModel = static_cast<CModel*>(m_pOwner->Get_Component(L"Com_Model"));
	CTransform* pTransform = static_cast<CTransform*>(m_pOwner->Get_Component(L"Com_Transform"));

	pModel->Set_AnimByIndex(CLINT_ANIM_RUN, UPPER);
	pModel->Set_AnimByIndex(CLINT_ANIM_RUN, LOWER);
	if (pGameInstance->Get_DIKeyState(DIK_W))
	{
		pTransform->Go_Direction(TimeDelta, CTransform::DIR_N);
		pTransform->Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(0.f));
	}
	else if (pGameInstance->Get_DIKeyState(DIK_A))
	{
		pTransform->Go_Direction(TimeDelta, CTransform::DIR_W);
		pTransform->Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(-90.f));
	}
	else if (pGameInstance->Get_DIKeyState(DIK_S))
	{
		pTransform->Go_Direction(TimeDelta, CTransform::DIR_S);
		pTransform->Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(180.f));
	}
	else if (pGameInstance->Get_DIKeyState(DIK_D))
	{
		pTransform->Go_Direction(TimeDelta, CTransform::DIR_E);
		pTransform->Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(90.f));
	}
	else
	{
		m_pStateContext->TransitionTo(L"ClintIdle");
	}

	if (pGameInstance->Get_DIKeyState(DIK_SPACE))
	{
		if (pGameInstance->Get_DIKeyState(DIK_W))
		{
			pModel->RootMotion(TimeDelta, CTransform::DIR_N);
		}
		else if (pGameInstance->Get_DIKeyState(DIK_A))
		{
			pModel->RootMotion(TimeDelta, CTransform::DIR_W);
		}
		else if (pGameInstance->Get_DIKeyState(DIK_S))
		{
			pModel->RootMotion(TimeDelta, CTransform::DIR_S);
		}
		else if (pGameInstance->Get_DIKeyState(DIK_D))
		{
			pModel->RootMotion(TimeDelta, CTransform::DIR_E);
		}

		m_pStateContext->TransitionTo(L"ClintDash");
	}
	else if (pGameInstance->Get_DIMouseState(CInput_Device::DIMK_LBUTTON))
	{
		m_pStateContext->TransitionTo(L"ClintShoot");
	}

	Safe_Release(pGameInstance);
}

void ClintRun::OnStateExit()
{
	__super::OnStateExit();
}

StateMachine<Clint, CLINT_ANIM>* ClintRun::Clone(void* pArg)
{
	__super::Initialize(pArg);
	return new ClintRun(*this);
}

ClintRun* ClintRun::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	return new ClintRun(pDevice, pContext);
}

void ClintRun::Free()
{
	__super::Free();
}