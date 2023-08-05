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
	, m_pInstanceSpeed(rhs.m_pInstanceSpeed)
	, m_pInstancePos(rhs.m_pInstancePos)
	, m_pLifeTime(rhs.m_pLifeTime)
{

}

HRESULT CVIBuffer_Instancing::Initialize_Prototype(const INSTANCEDESC* pInstanceDesc)
{
	m_InstanceDesc = *(INSTANCEDESC*)pInstanceDesc;

	m_pInstanceSpeed = new _float[m_InstanceDesc.iNumInstance];
	m_pInstancePos = new _float4[m_InstanceDesc.iNumInstance];
	m_pLifeTime = new _double[m_InstanceDesc.iNumInstance];
	ZeroMemory(m_pLifeTime, sizeof(_double) * m_InstanceDesc.iNumInstance);

	for (size_t i = 0; i < m_InstanceDesc.iNumInstance; i++)
	{
		// 랜덤범위를 MIN~MAX로 잡음 EX) uint2(2, 10)으로 넣은 경우 범위가 2~10으로 계산됨.
		m_pInstanceSpeed[i] = rand() % (m_InstanceDesc.vSpeed.y + 1 - m_InstanceDesc.vSpeed.x) + m_InstanceDesc.vSpeed.x;
		
		// x,z의 경우 Range = 10으로 들어오면 [-5,5]로 범위를 잡아준다.
		// y의 경우 Range = 4, Height = 10으로 들어오면  [-2 + 10 = 8, 2 + 10 = 12]로 범위를 잡아준다.
		m_pInstancePos[i] = _float4(rand() % (_uint(m_InstanceDesc.vRange.x) + 1) - m_InstanceDesc.vRange.x * 0.5f,
			rand() % (_uint(m_InstanceDesc.vRange.y) + 1) - m_InstanceDesc.vRange.y * 0.5f + m_InstanceDesc.fHeight,
			rand() % (_uint(m_InstanceDesc.vRange.z) + 1) - m_InstanceDesc.vRange.z * 0.5f,
			1.f);
	}

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
		pVertices[i].vTranslation = m_pInstancePos[i];
	}

	D3D11_SUBRESOURCE_DATA		SubResourceData;
	ZeroMemory(&SubResourceData, sizeof SubResourceData);

	SubResourceData.pSysMem = pVertices;

	if (FAILED(m_pDevice->CreateBuffer(&BufferDesc, &SubResourceData, &m_pVBInstance)))
		return E_FAIL;

	Safe_Delete_Array(pVertices);

	return S_OK;
}

void CVIBuffer_Instancing::Tick(_double TimeDelta)
{

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

	if (false == m_isCloned)
	{
		Safe_Delete_Array(m_pInstanceSpeed);
		Safe_Delete_Array(m_pInstancePos);
		Safe_Delete_Array(m_pLifeTime);
	}

	Safe_Release(m_pVBInstance);

}
