#include "..\Public\VIBuffer_Instancing.h"

CVIBuffer_Instancing::CVIBuffer_Instancing(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CVIBuffer(pDevice, pContext)
{
}

CVIBuffer_Instancing::CVIBuffer_Instancing(const CVIBuffer_Instancing& rhs)
	: CVIBuffer(rhs)
	, m_InstanceDesc(rhs.m_InstanceDesc)
	, m_iInstanceStride(rhs.m_iInstanceStride)
	, m_iIndexCountPerInstance(rhs.m_iIndexCountPerInstance)
{

}

HRESULT CVIBuffer_Instancing::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CVIBuffer_Instancing::Initialize(void* pArg)
{
	if (nullptr != pArg)
		memcpy(&m_InstanceDesc, pArg, sizeof m_InstanceDesc);

	/* 인스턴스 정점 버퍼를 생성한다.(m_pVBInstance) */
	D3D11_BUFFER_DESC		BufferDesc;
	ZeroMemory(&BufferDesc, sizeof BufferDesc);

	m_iInstanceStride = sizeof(VTXINSTANCE);

	BufferDesc.ByteWidth = { m_iInstanceStride * m_InstanceDesc.iNumInstance };
	BufferDesc.Usage = { D3D11_USAGE_DYNAMIC }; // 
	BufferDesc.BindFlags = { D3D11_BIND_VERTEX_BUFFER }; //
	BufferDesc.StructureByteStride = { m_iInstanceStride };
	BufferDesc.CPUAccessFlags = { D3D11_CPU_ACCESS_WRITE };
	BufferDesc.MiscFlags = { 0 };

	VTXINSTANCE* pVertices = new VTXINSTANCE[m_InstanceDesc.iNumInstance];

	for (size_t i = 0; i < m_InstanceDesc.iNumInstance; ++i)
	{
		pVertices[i].vRight = _float4(1.f, 0.f, 0.f, 0.f);
		pVertices[i].vUp = _float4(0.f, 1.f, 0.f, 0.f);
		pVertices[i].vLook = _float4(0.f, 0.f, 1.f, 0.f);
		pVertices[i].vTranslation = _float4(0.f ,0.f, 0.f, 1.f);
	}

	D3D11_SUBRESOURCE_DATA		SubResourceData;
	ZeroMemory(&SubResourceData, sizeof SubResourceData);

	SubResourceData.pSysMem = pVertices;

	if (FAILED(m_pDevice->CreateBuffer(&BufferDesc, &SubResourceData, &m_pVBInstance)))
		return E_FAIL;

	Safe_Delete_Array(pVertices);

	return S_OK;
}

void CVIBuffer_Instancing::Tick(list<_float4x4>& InstMatrices, _double TimeDelta)
{
	D3D11_MAPPED_SUBRESOURCE		SubResource;

	m_pContext->Map(m_pVBInstance, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &SubResource);
	VTXINSTANCE* pVertices = static_cast<VTXINSTANCE*>(SubResource.pData);

	_uint i = 0;
	for (auto iter = InstMatrices.begin(); iter != InstMatrices.end(); ++iter)
	{
		memcpy(&pVertices[i].vRight, &(*iter)._11, sizeof(_float4));
		memcpy(&pVertices[i].vUp, &(*iter)._21, sizeof(_float4));
		memcpy(&pVertices[i].vLook, &(*iter)._31, sizeof(_float4));
		memcpy(&pVertices[i].vTranslation, &(*iter)._41, sizeof(_float4));
		++i;
	}

	m_pContext->Unmap(m_pVBInstance, 0);
}

HRESULT CVIBuffer_Instancing::Render()
{
	if (nullptr == m_pContext)
		return E_FAIL;

	ID3D11Buffer* pBuffers[] = {
		m_pVB,
		m_pVBInstance,
	};

	_uint					iStrides[] = {
		m_iStride,
		m_iInstanceStride,
	};

	_uint					iOffset[] = {
		0,
		0,
	};

	m_pContext->IASetVertexBuffers(0, m_iNumVertexBuffers, pBuffers, iStrides, iOffset);
	m_pContext->IASetIndexBuffer(m_pIB, m_eFormat, 0);
	m_pContext->IASetPrimitiveTopology(m_eTopology);
	m_pContext->DrawIndexedInstanced(m_iIndexCountPerInstance, m_InstanceDesc.iNumInstance, 0, 0, 0);

	return S_OK;
}

void CVIBuffer_Instancing::Free()
{
	__super::Free();

	Safe_Release(m_pVBInstance);

}
