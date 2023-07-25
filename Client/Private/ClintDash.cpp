#include "..\Public\ClintDash.h"
#include "GameInstance.h"
#include "Clint.h"

ClintDash::ClintDash(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: StateMachine<Clint, CLINT_ANIM>(pDevice, pContext)
{
}

ClintDash::ClintDash(const ClintDash& rhs)
	: StateMachine<Clint, CLINT_ANIM>(rhs)
{
}

void ClintDash::OnStateEnter()
{
	__super::OnStateEnter();

	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);	

	CModel* pModel = static_cast<CModel*>(m_pOwner->Get_Component(L"Com_Model"));
	CTransform* pTransform = static_cast<CTransform*>(m_pOwner->Get_Component(L"Com_Transform"));

	SetDashDir(pModel, pTransform);

	pModel->Set_AnimByIndex(CLINT_DASH, UPPER);
	pModel->Set_AnimByIndex(CLINT_DASH, LOWER);
	Safe_Release(pGameInstance);
}

void ClintDash::OnStateTick(_double TimeDelta)
{
	__super::OnStateTick(TimeDelta);

	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);
	CModel* pModel = static_cast<CModel*>(m_pOwner->Get_Component(L"Com_Model"));
	CTransform* pTransform = static_cast<CTransform*>(m_pOwner->Get_Component(L"Com_Transform"));

	if (pModel->IsAnimationFinished(LOWER))
	{
		m_pStateContext->TransitionTo(L"ClintIdle");
	}

	pModel->RootMotion(TimeDelta, pTransform->GetCurDirection());

	Safe_Release(pGameInstance);
}

void ClintDash::OnStateExit()
{
	__super::OnStateExit();
}

void ClintDash::SetDashDir(CModel* pModel, CTransform* pTransform)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);
	
	_byte W = pGameInstance->Get_DIKeyState(DIK_W);
	_byte A = pGameInstance->Get_DIKeyState(DIK_A);
	_byte S = pGameInstance->Get_DIKeyState(DIK_S);
	_byte D = pGameInstance->Get_DIKeyState(DIK_D);
	if (W)
	{
		if (D)
		{
			pTransform->Rotation(WorldAxisY(), XMConvertToRadians(45.f));
			pModel->RootMotion(0.016, CTransform::DIR_NE);
		}
		else if (A)
		{
			pTransform->Rotation(WorldAxisY(), XMConvertToRadians(315.f));
			pModel->RootMotion(0.016, CTransform::DIR_NW);
		}
		else
		{
			pTransform->Rotation(WorldAxisY(), XMConvertToRadians(0.f));
			pModel->RootMotion(0.016, CTransform::DIR_N);
		}
	}
	else if (A)
	{
		if (W)
		{
			pTransform->Rotation(WorldAxisY(), XMConvertToRadians(315.f));
			pModel->RootMotion(0.016, CTransform::DIR_NW);
		}
		else if (S)
		{
			pTransform->Rotation(WorldAxisY(), XMConvertToRadians(225.f));
			pModel->RootMotion(0.016, CTransform::DIR_SW);
		}
		else
		{
			pTransform->Rotation(WorldAxisY(), XMConvertToRadians(270.f));
			pModel->RootMotion(0.016, CTransform::DIR_W);
		}
	}
	else if (S)
	{
		if (D)
		{
			pTransform->Rotation(WorldAxisY(), XMConvertToRadians(135.f));
			pModel->RootMotion(0.016, CTransform::DIR_SE);
		}
		else if (A)
		{
			pTransform->Rotation(WorldAxisY(), XMConvertToRadians(225.f));
			pModel->RootMotion(0.016, CTransform::DIR_SW);
		}
		else
		{
			pTransform->Rotation(WorldAxisY(), XMConvertToRadians(180.f));
			pModel->RootMotion(0.016, CTransform::DIR_S);
		}
	}
	else if (D)
	{
		if (W)
		{
			pTransform->Rotation(WorldAxisY(), XMConvertToRadians(45.f));
			pModel->RootMotion(0.016, CTransform::DIR_NE);
		}
		else if (S)
		{
			pTransform->Rotation(WorldAxisY(), XMConvertToRadians(135.f));
			pModel->RootMotion(0.016, CTransform::DIR_SE);
		}
		else
		{
			pTransform->Rotation(WorldAxisY(), XMConvertToRadians(90.f));
			pModel->RootMotion(0.016, CTransform::DIR_E);
		}
	}
	Safe_Release(pGameInstance);
}

StateMachine<Clint, CLINT_ANIM>* ClintDash::Clone(void* pArg)
{
	__super::Initialize(pArg);
	return new ClintDash(*this);
}

ClintDash* ClintDash::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	return new ClintDash(pDevice, pContext);
}

void ClintDash::Free()
{
	__super::Free();
}