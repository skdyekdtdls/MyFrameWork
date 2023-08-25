#include "VIBuffer_Sphere.h"

CVIBuffer_Sphere::CVIBuffer_Sphere(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CVIBuffer(pDevice, pContext)
{
}

CVIBuffer_Sphere::CVIBuffer_Sphere(const CVIBuffer_Sphere& rhs)
	: CVIBuffer(rhs)
{
}

HRESULT CVIBuffer_Sphere::Initialize_Prototype(_uint iNumSegments, _uint iNumRings, _float fRadius)
{
	m_iNumVertexBuffers = { 1 };
	m_iStride = { sizeof(VTXPOSNORTEX) };
	m_iNumVertices = { (iNumSegments + 1) * (iNumRings + 1) };
	m_iIndexStride = { sizeof _ushort };
	m_iNumIndices = { 6 * iNumSegments * iNumRings };
	m_eFormat = { DXGI_FORMAT_R16_UINT };
	m_eTopology = { D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST };

	ZeroMemory(&m_BufferDesc, sizeof m_BufferDesc);
	m_BufferDesc.ByteWidth = { m_iStride * m_iNumVertices };
	m_BufferDesc.Usage = { D3D11_USAGE_DEFAULT };
	m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_BufferDesc.StructureByteStride = { m_iStride };
	m_BufferDesc.CPUAccessFlags = { 0 };
	m_BufferDesc.MiscFlags = { 0 };

	VTXPOSNORTEX* pVertices = new VTXPOSNORTEX[m_iNumVertices];
	ZeroMemory(pVertices, sizeof(VTXPOSNORTEX) * m_iNumVertices);

	for (_uint r = 0; r <= iNumRings; r++) {
		for (_uint s = 0; s <= iNumSegments; s++) {
			const float y = sin(-(XM_PI * 0.5f) + XM_PI * r / iNumRings);
			const float x = cos(2 * XM_PI * s / iNumSegments) * sin(XM_PI * r / iNumRings);
			const float z = sin(2 * XM_PI * s / iNumSegments) * sin(XM_PI * r / iNumRings);

			_float3 position = { x * fRadius, y * fRadius, z * fRadius };
			pVertices[r * (iNumSegments + 1) + s].vPosition = position;
			pVertices[r * (iNumSegments + 1) + s].vTexCoord = { _float2((float)s / iNumSegments, (float)r / iNumRings) };

			// 노말을 설정합니다. 여기서 노말은 정규화된 위치 벡터입니다.
			XMStoreFloat3(&pVertices[r * (iNumSegments + 1) + s].vNormal,  XMVector3Normalize(XMLoadFloat3(&position))); // 가정: _float3에 대한 연산자 오버로딩이 적절히 정의되어 있음
		}
	}


	ZeroMemory(&m_SubResourceData, sizeof m_SubResourceData);
	m_SubResourceData.pSysMem = { pVertices };

	if (FAILED(__super::Create_Buffer(&m_pVB)))
		return E_FAIL;

	Safe_Delete_Array(pVertices);

	ZeroMemory(&m_BufferDesc, sizeof m_BufferDesc);
	m_BufferDesc.ByteWidth = { m_iIndexStride * m_iNumIndices };
	m_BufferDesc.Usage = { D3D11_USAGE_DEFAULT };
	m_BufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	m_BufferDesc.StructureByteStride = { 0 };
	m_BufferDesc.CPUAccessFlags = { 0 };
	m_BufferDesc.MiscFlags = { 0 };

	_ushort* pIndices = new _ushort[m_iNumIndices];
	ZeroMemory(pIndices, sizeof(_ushort) * m_iNumIndices);

	_uint iIndex = 0;
	for (_uint r = 0; r < iNumRings; r++) {
		for (_uint s = 0; s < iNumSegments; s++) {
			_uint iCurRow = r * (iNumSegments + 1);
			_uint iNextRow = (r + 1) * (iNumSegments + 1);

			// 1st triangle
			pIndices[iIndex++] = iCurRow + s;
			pIndices[iIndex++] = iNextRow + s;
			pIndices[iIndex++] = iNextRow + s + 1;

			// 2nd triangle
			pIndices[iIndex++] = iCurRow + s;
			pIndices[iIndex++] = iNextRow + s + 1;
			pIndices[iIndex++] = iCurRow + s + 1;
		}
	}

	ZeroMemory(&m_SubResourceData, sizeof D3D11_SUBRESOURCE_DATA);
	m_SubResourceData.pSysMem = pIndices;

	FAILED_CHECK_RETURN(__super::Create_Buffer(&m_pIB), E_FAIL);

	Safe_Delete_Array(pIndices);

	return S_OK;
}

HRESULT CVIBuffer_Sphere::Initialize(void* pArg)
{
	__super::Initialize(pArg);
	return S_OK;
}

CVIBuffer_Sphere* CVIBuffer_Sphere::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, _uint iNumSegments, _uint iNumRings, _float fRadius)
{
	CVIBuffer_Sphere* pInstance = new CVIBuffer_Sphere(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(iNumSegments, iNumRings, fRadius)))
	{
		MSG_BOX("Failed to Created CVIBuffer_Sphere");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CComponent* CVIBuffer_Sphere::Clone(void* pArg)
{
	CVIBuffer_Sphere* pInstance = new CVIBuffer_Sphere(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned CVIBuffer_Sphere");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CVIBuffer_Sphere::Free(void)
{
	__super::Free();
}
