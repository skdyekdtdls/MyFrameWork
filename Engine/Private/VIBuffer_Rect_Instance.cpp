#include "..\Public\VIBuffer_Rect_Instance.h"

CVIBuffer_Rect_Instance::CVIBuffer_Rect_Instance(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CVIBuffer_Instancing(pDevice, pContext)
{

}

CVIBuffer_Rect_Instance::CVIBuffer_Rect_Instance(const CVIBuffer_Rect_Instance& rhs)
	: CVIBuffer_Instancing(rhs)
{

}

HRESULT CVIBuffer_Rect_Instance::Initialize_Prototype(const CVIBuffer_Instancing::CVIBUFFER_INSTANCE_DESC* pInstanceDesc)
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	m_iIndexCountPerInstance = 6;
	m_iNumVertexBuffers = { 2 };
	m_iStride = { sizeof(VTXPOSTEX) };
	m_iNumVertices = { 4 };
	m_iIndexStride = { sizeof(_ushort) };
	m_iNumIndices = { m_iIndexCountPerInstance * m_InstanceDesc.iNumInstance };
	m_eFormat = DXGI_FORMAT_R16_UINT;
	m_eTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

#pragma region VERTEX_BUFFER
	ZeroMemory(&m_BufferDesc, sizeof m_BufferDesc);

	m_BufferDesc.ByteWidth = { m_iStride * m_iNumVertices };
	m_BufferDesc.Usage = { D3D11_USAGE_DEFAULT };
	m_BufferDesc.BindFlags = { D3D11_BIND_VERTEX_BUFFER };
	m_BufferDesc.StructureByteStride = { m_iStride };
	m_BufferDesc.CPUAccessFlags = { 0 };
	m_BufferDesc.MiscFlags = { 0 };


	VTXPOSTEX* pVertices = new VTXPOSTEX[m_iNumVertices];
	ZeroMemory(pVertices, sizeof(VTXPOSTEX) * m_iNumVertices);

	pVertices[0].vPosition = _float3(-0.5f, 0.5f, 0.f);
	pVertices[0].vTexCoord = _float2(0.f, 0.f);

	pVertices[1].vPosition = _float3(0.5f, 0.5f, 0.f);
	pVertices[1].vTexCoord = _float2(1.f, 0.f);

	pVertices[2].vPosition = _float3(0.5f, -0.5f, 0.f);
	pVertices[2].vTexCoord = _float2(1.f, 1.f);

	pVertices[3].vPosition = _float3(-0.5f, -0.5f, 0.f);
	pVertices[3].vTexCoord = _float2(0.f, 1.f);

	ZeroMemory(&m_SubResourceData, sizeof m_SubResourceData);
	m_SubResourceData.pSysMem = pVertices;

	if (FAILED(__super::Create_Buffer(&m_pVB)))
		return E_FAIL;
	Safe_Delete_Array(pVertices);

#pragma endregion


#pragma region INDEX_BUFFER
	ZeroMemory(&m_BufferDesc, sizeof m_BufferDesc);

	m_BufferDesc.ByteWidth = { m_iIndexStride * m_iNumIndices };
	m_BufferDesc.Usage = { D3D11_USAGE_DEFAULT };
	m_BufferDesc.BindFlags = { D3D11_BIND_INDEX_BUFFER };
	m_BufferDesc.StructureByteStride = { 0 };
	m_BufferDesc.CPUAccessFlags = { 0 };
	m_BufferDesc.MiscFlags = { 0 };

	_ushort* pIndices = new _ushort[m_iNumIndices];
	ZeroMemory(pIndices, sizeof(_ushort) * m_iNumIndices);

	_uint		iNumIndices = { 0 };

	for (size_t i = 0; i < m_InstanceDesc.iNumInstance; i++)
	{
		pIndices[iNumIndices++] = 0;
		pIndices[iNumIndices++] = 1;
		pIndices[iNumIndices++] = 2;

		pIndices[iNumIndices++] = 0;
		pIndices[iNumIndices++] = 2;
		pIndices[iNumIndices++] = 3;
	}

	ZeroMemory(&m_SubResourceData, sizeof m_SubResourceData);
	m_SubResourceData.pSysMem = pIndices;

	if (FAILED(__super::Create_Buffer(&m_pIB)))
		return E_FAIL;
	Safe_Delete_Array(pIndices);
#pragma endregion


	return S_OK;
}

HRESULT CVIBuffer_Rect_Instance::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

void CVIBuffer_Rect_Instance::Tick(list<_float4x4>& InstMatrices, _double TimeDelta)
{
	D3D11_MAPPED_SUBRESOURCE		SubResource;

	m_pContext->Map(m_pVBInstance, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &SubResource);

	//for (_uint i = 0; i < m_InstanceDesc.iNumInstance; ++i)
	//{
	//	((VTXINSTANCE*)SubResource.pData)[i].vTranslation.y -= m_pInstanceSpeed[i] * TimeDelta;

	//	if (0 >= ((VTXINSTANCE*)SubResource.pData)[i].vTranslation.y)
	//		((VTXINSTANCE*)SubResource.pData)[i].vTranslation.y = m_pInstancePos[i].y;
	//}

	m_pContext->Unmap(m_pVBInstance, 0);
}

CVIBuffer_Rect_Instance* CVIBuffer_Rect_Instance::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const CVIBuffer_Instancing::CVIBUFFER_INSTANCE_DESC* pInstanceDesc)
{
	CVIBuffer_Rect_Instance* pInstance = new CVIBuffer_Rect_Instance(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(pInstanceDesc)))
	{
		MSG_BOX("Failed to Created CVIBuffer_Rect_Instance");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CComponent* CVIBuffer_Rect_Instance::Clone(void* pArg)
{
	CVIBuffer_Rect_Instance* pInstance = new CVIBuffer_Rect_Instance(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned CVIBuffer_Rect_Instance");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CVIBuffer_Rect_Instance::Free()
{
	__super::Free();

}