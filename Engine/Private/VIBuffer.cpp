#include "..\Public\VIBuffer.h"

CVIBuffer::CVIBuffer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CComponent(pDevice, pContext)
{
}

CVIBuffer::CVIBuffer(const CVIBuffer& rhs)
	: CComponent(rhs)
{
}

HRESULT CVIBuffer::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CVIBuffer::Initialize(void* pArg)
{
	return S_OK;
}

HRESULT CVIBuffer::Create_Buffer(ID3D11Buffer** ppOut)
{
	return m_pDevice->CreateBuffer(&m_BufferDesc, &m_SubResourceData, ppOut);
}

void CVIBuffer::Free()
{
	__super::Free();
}