#include "..\Public\ClintAnimState.h"

ClintAnimState::ClintAnimState(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: AnimState(pDevice, pContext)
{

}

HRESULT ClintAnimState::Initialize(Clint* pOwner)
{
	//m_pOwner = pOwner;
	return S_OK;
}

void ClintAnimState::Tick(_double TimeDelta)
{
}

void ClintAnimState::Late_Tick(_double TimeDelta)
{
}

void ClintAnimState::Free()
{
	__super::Free();
}
