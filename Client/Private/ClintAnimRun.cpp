#include "..\Public\ClintAnimRun.h"
#include "GameInstance.h"
#include "Clint.h"

ClintAnimRun::ClintAnimRun(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: ClintAnimState(pDevice, pContext)
{

}

HRESULT ClintAnimRun::Initialize(Clint* pOwner)
{
	__super::Initialize(pOwner);

	return S_OK;
}

void ClintAnimRun::Tick(_double TimeDelta)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);
	CTransform* pTransform = m_pOwner->GetComponent<CTransform>();
	CModel* pModel = m_pOwner->GetComponent<CModel>();
	if (pGameInstance->Get_DIKeyState(DIK_W))
	{
		pTransform->Go_Direction(TimeDelta, CTransform::DIR_N);
		pTransform->Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(0.f));
	}

	if (pGameInstance->Get_DIKeyState(DIK_A))
	{
		pTransform->Go_Direction(TimeDelta, CTransform::DIR_W);
		pTransform->Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(-90.f));
	}

	if(pGameInstance->Get_DIKeyState(DIK_S))
	{
		pTransform->Go_Direction(TimeDelta, CTransform::DIR_S);
		pTransform->Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(180.f));
	}

	if (pGameInstance->Get_DIKeyState(DIK_D))
	{
		pTransform->Go_Direction(TimeDelta, CTransform::DIR_E);
		pTransform->Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(90.f));
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

		m_pOwner->Set_ClintAnimState(CLINT_ANIM::DASH, UPPER);
		m_pOwner->Set_ClintAnimState(CLINT_ANIM::DASH, LOWER);
	}

	Safe_Release(pGameInstance);
}

void ClintAnimRun::Late_Tick(_double TimeDelta)
{
}

ClintAnimRun* ClintAnimRun::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, Clint* pClint)
{
	ClintAnimRun* pInstance = new ClintAnimRun(pDevice, pContext);

	if (pInstance->Initialize(pClint))
	{
		MSG_BOX("Failed to Create ClintAnimRun");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void ClintAnimRun::Free()
{
	__super::Free();
}
