#include "AnimState.h"

AnimState::AnimState(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: m_pDevice(pDevice)
	, m_pContext(pContext)
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

void AnimState::Free()
{
	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
}

