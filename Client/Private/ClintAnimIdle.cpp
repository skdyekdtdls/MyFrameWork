#include "..\Public\ClintAnimIdle.h"
#include "GameInstance.h"
#include "Clint.h"

ClintAnimIdle::ClintAnimIdle(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: ClintAnimState(pDevice, pContext)
{

}

HRESULT ClintAnimIdle::Initialize(Clint* pOwner)
{
	__super::Initialize(pOwner);

	return S_OK;
}

void ClintAnimIdle::Tick(_double TimeDelta)
{

}

void ClintAnimIdle::Late_Tick(_double TimeDelta)
{
}

ClintAnimIdle* ClintAnimIdle::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, Clint* pClint)
{
	ClintAnimIdle* pInstance = new ClintAnimIdle(pDevice, pContext);
	
	if (pInstance->Initialize(pClint))
	{
		MSG_BOX("Failed to Create ClintAnimIdle");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void ClintAnimIdle::Free()
{
	__super::Free();
}
