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
