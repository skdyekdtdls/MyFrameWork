#include "VIBuffer_Rect.h"

CVIBuffer_Rect::CVIBuffer_Rect(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CVIBuffer(pDevice, pContext)
{
}

CVIBuffer_Rect::CVIBuffer_Rect(const CVIBuffer_Rect& rhs)
	: CVIBuffer(rhs)
{
}

HRESULT CVIBuffer_Rect::Initialize_Prototype()
{
	m_iNumVertices = { 4 };
	m_iStride = { sizeof(VTXPOSTEX) };

	ZeroMemory(&m_BufferDesc, sizeof m_BufferDesc);
	m_BufferDesc.ByteWidth = { m_iStride * m_iNumVertices };
	m_BufferDesc.Usage = { D3D11_USAGE_DEFAULT };
	m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_BufferDesc.StructureByteStride = { m_iStride };
	m_BufferDesc.CPUAccessFlags = { 0 };
	m_BufferDesc.MiscFlags = { 0 };
	
	VTXPOSTEX* pVertices = new VTXPOSTEX[m_iNumVertices];
	ZeroMemory(&pVertices, sizeof(pVertices) * m_iNumVertices);

	pVertices->vPostion = { _float3(-0.5f, 0.5f, 0.f) };
	pVertices->vTexCoord = { _float2(0.f, 0.f)};

	pVertices->vPostion = { _float3(_float3(0.5f, 0.5f, 0.f)) };
	pVertices->vTexCoord = { _float2(1.f, 0.f) };

	pVertices->vPostion = { _float3(_float3(0.5f, -0.5f, 0.f)) };
	pVertices->vTexCoord = { _float2(1.f, 1.f) };

	pVertices->vPostion = { _float3(_float3(-0.5f, -0.5f, 0.f)) };
	pVertices->vTexCoord = { _float2(0.f, 1.f) };

	ZeroMemory(&m_SubResourceData, sizeof m_SubResourceData);
	m_SubResourceData.pSysMem = { pVertices };

	if (FAILED(__super::Create_Buffer(&m_pVB)))
		return E_FAIL;

	//Safe_Delete_Array(pVertices);

	return S_OK;
}

HRESULT CVIBuffer_Rect::Initialize(void* pArg)
{
	return S_OK;
}

CVIBuffer_Rect* CVIBuffer_Rect::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CVIBuffer_Rect* pInstance = new CVIBuffer_Rect(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created CVIBuffer_Rect");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CComponent* CVIBuffer_Rect::Clone(void* pArg)
{
	CVIBuffer_Rect* pInstance = new CVIBuffer_Rect(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned CVIBuffer_Rect");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CVIBuffer_Rect::Free(void)
{
	__super::Free();
}
