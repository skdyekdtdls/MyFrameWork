#include "..\Public\Component.h"
#include "GameObject.h"
CComponent::CComponent(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: m_pDevice(pDevice)
	, m_pContext(pContext)
	, m_isCloned(false)
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

CComponent::CComponent(const CComponent& rhs)
	: m_pDevice(rhs.m_pDevice)
	, m_pContext(rhs.m_pContext)
	, m_isCloned(true)
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

HRESULT CComponent::Initialize_Prototype(void)
{
	return S_OK;
}

HRESULT CComponent::Initialize(void* pArg)
{
	if(nullptr != pArg)
		m_pOwner = ((COMPONENT_DESC*)pArg)->pOwner;

	return S_OK;
}

void CComponent::Free(void)
{
	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
}
