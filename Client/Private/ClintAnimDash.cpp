#include "..\Public\ClintAnimDash.h"
#include "GameInstance.h"
#include "Clint.h"

ClintAnimDash::ClintAnimDash(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: ClintAnimState(pDevice, pContext)
{

}

HRESULT ClintAnimDash::Initialize(Clint* pOwner)
{
	__super::Initialize(pOwner);

	return S_OK;
}

void ClintAnimDash::Tick(_double TimeDelta)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	CModel* pModel = m_pOwner->GetComponent<CModel>();
	
	if (pModel->IsAnimationFinished())
	{
		m_pOwner->Set_ClintAnimState(CLINT_ANIM::IDLE);
	}

	CTransform* pTransform = m_pOwner->GetComponent<CTransform>();
	pModel->RootMotion(TimeDelta, pTransform->GetCurDirection());
	

	Safe_Release(pGameInstance);
}

void ClintAnimDash::Late_Tick(_double TimeDelta)
{
}

ClintAnimDash* ClintAnimDash::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, Clint* pClint)
{
	ClintAnimDash* pInstance = new ClintAnimDash(pDevice, pContext);

	if (pInstance->Initialize(pClint))
	{
		MSG_BOX("Failed to Create ClintAnimDash");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void ClintAnimDash::Free()
{
	__super::Free();
}
