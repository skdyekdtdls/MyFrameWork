#include "..\Public\VIBuffer.h"

CVIBuffer::CVIBuffer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CComponent(pDevice, pContext)
{
}

CVIBuffer::CVIBuffer(const CVIBuffer& rhs)
	: CComponent(rhs)
	, m_pVB(rhs.m_pVB)
	, m_pIB(rhs.m_pIB)
	, m_BufferDesc(rhs.m_BufferDesc)
	, m_SubResourceData(rhs.m_SubResourceData)
	, m_iStride(rhs.m_iStride)
	, m_iNumVertices(rhs.m_iNumVertices)
	, m_iNumVertexBuffers(rhs.m_iNumVertexBuffers)
	, m_iNumIndices(rhs.m_iNumIndices)
	, m_iIndexStride(rhs.m_iIndexStride)
	, m_eFormat(rhs.m_eFormat)
	, m_eTopology(rhs.m_eTopology)
{
	Safe_AddRef(m_pVB);
	Safe_AddRef(m_pIB);
}

void CVIBuffer::SaveAssimp(HANDLE hFile, DWORD& dwByte)
{
	WriteVoid(&m_iStride, sizeof(_uint));
	WriteVoid(&m_iNumVertices, sizeof(_uint));
	WriteVoid(&m_iNumIndices, sizeof(_uint));
	WriteVoid(&m_iIndexStride, sizeof(_uint));
	WriteVoid(&m_eFormat, sizeof(DXGI_FORMAT));
	WriteVoid(&m_iNumVertexBuffers, sizeof(_uint));
	WriteVoid(&m_eTopology, sizeof(D3D11_PRIMITIVE_TOPOLOGY));
}

void CVIBuffer::LoadAssimp(HANDLE hFile, DWORD& dwByte)
{
	ReadVoid(&m_iStride, sizeof(_uint));
	ReadVoid(&m_iNumVertices, sizeof(_uint));
	ReadVoid(&m_iNumIndices, sizeof(_uint));
	ReadVoid(&m_iIndexStride, sizeof(_uint));
	ReadVoid(&m_eFormat, sizeof(DXGI_FORMAT));
	ReadVoid(&m_iNumVertexBuffers, sizeof(_uint));
	ReadVoid(&m_eTopology, sizeof(D3D11_PRIMITIVE_TOPOLOGY));
}

HRESULT CVIBuffer::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CVIBuffer::Initialize(void* pArg)
{
	__super::Initialize(pArg);
	return S_OK;
}

HRESULT CVIBuffer::Render()
{
	if (nullptr == m_pContext)
		return E_FAIL;

	ID3D11Buffer* pBuffers[] = {
		m_pVB,
	};

	_uint	iStrides[] = {
		m_iStride,
	};

	_uint iOffset[] = {
		0,
	};

	m_pContext->IASetVertexBuffers(0, m_iNumVertexBuffers, pBuffers, iStrides, iOffset);
	m_pContext->IASetIndexBuffer(m_pIB, m_eFormat, 0);
	m_pContext->IASetPrimitiveTopology(m_eTopology);
	m_pContext->DrawIndexed(m_iNumIndices, 0, 0);

	return S_OK;
}

HRESULT CVIBuffer::Create_Buffer(ID3D11Buffer** ppOut)
{
	return m_pDevice->CreateBuffer(&m_BufferDesc, &m_SubResourceData, ppOut);
}

void CVIBuffer::Free()
{
	Safe_Release(m_pVB);
	Safe_Release(m_pIB);
	__super::Free();
}