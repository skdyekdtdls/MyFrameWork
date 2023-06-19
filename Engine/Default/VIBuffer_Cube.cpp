#include "VIBuffer_Cube.h"

CVIBuffer_Cube::CVIBuffer_Cube(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CVIBuffer(pDevice, pContext)
{
}

CVIBuffer_Cube::CVIBuffer_Cube(const CVIBuffer_Cube& rhs)
	: CVIBuffer(rhs)
{

}

HRESULT CVIBuffer_Cube::Initialize_Prototype()
{
	m_iNumVertexBuffers = { 1 };
	m_iStride = { sizeof(VTXPOSCOL) };
	m_iNumVertices = { 8 };
	m_iIndexStride = { sizeof _ushort };
	m_iNumIndices = { 36 };
	m_eFormat = { DXGI_FORMAT_R16_UINT };
	m_eTopology = { D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST };

	ZeroMemory(&m_BufferDesc, sizeof m_BufferDesc);
	m_BufferDesc.ByteWidth = { m_iStride * m_iNumVertices };
	m_BufferDesc.Usage = { D3D11_USAGE_DEFAULT };
	m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_BufferDesc.StructureByteStride = { m_iStride };
	m_BufferDesc.CPUAccessFlags = { 0 };
	m_BufferDesc.MiscFlags = { 0 };

	VTXPOSCOL* pVertices = new VTXPOSCOL[m_iNumVertices];
	ZeroMemory(pVertices, sizeof(VTXPOSCOL) * m_iNumVertices);

	pVertices[0].vPosition = _float3(-0.5f, -0.5f, -0.5f); // bottom left back
	pVertices[1].vPosition = _float3(0.5f, -0.5f, -0.5f); // bottom right back
	pVertices[2].vPosition = _float3(-0.5f, 0.5f, -0.5f); // top left back
	pVertices[3].vPosition = _float3(0.5f, 0.5f, -0.5f); // top right back
	pVertices[4].vPosition = _float3(-0.5f, -0.5f, 0.5f); // bottom left front
	pVertices[5].vPosition = _float3(0.5f, -0.5f, 0.5f); // bottom right front
	pVertices[6].vPosition = _float3(-0.5f, 0.5f, 0.5f); // top left front
	pVertices[7].vPosition = _float3(0.5f, 0.5f, 0.5f); // top right front

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

	// back face
	pIndices[0] = 0; pIndices[1] = 1; pIndices[2] = 2;
	pIndices[3] = 2; pIndices[4] = 1; pIndices[5] = 3;

	// front face
	pIndices[6] = 4; pIndices[7] = 6; pIndices[8] = 5;
	pIndices[9] = 5; pIndices[10] = 6; pIndices[11] = 7;

	// left face
	pIndices[12] = 4; pIndices[13] = 0; pIndices[14] = 6;
	pIndices[15] = 6; pIndices[16] = 0; pIndices[17] = 2;

	// right face
	pIndices[18] = 1; pIndices[19] = 5; pIndices[20] = 3;
	pIndices[21] = 3; pIndices[22] = 5; pIndices[23] = 7;

	// bottom face
	pIndices[24] = 4; pIndices[25] = 5; pIndices[26] = 0;
	pIndices[27] = 0; pIndices[28] = 5; pIndices[29] = 1;

	// top face
	pIndices[30] = 2; pIndices[31] = 3; pIndices[32] = 6;
	pIndices[33] = 6; pIndices[34] = 3; pIndices[35] = 7;

	ZeroMemory(&m_SubResourceData, sizeof D3D11_SUBRESOURCE_DATA);
	m_SubResourceData.pSysMem = { pIndices };

	if (FAILED(__super::Create_Buffer(&m_pIB)))
		return E_FAIL;

	Safe_Delete_Array(pIndices);

	return S_OK;
}

HRESULT CVIBuffer_Cube::Initialize(void* pArg)
{
	return S_OK;
}


CVIBuffer_Cube* CVIBuffer_Cube::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CVIBuffer_Cube* pInstance = new CVIBuffer_Cube(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created CVIBuffer_Cube");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CVIBuffer_Cube::Clone(void* pArg)
{
	CVIBuffer_Cube* pInstance = new CVIBuffer_Cube(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned CVIBuffer_Cube");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CVIBuffer_Cube::Free()
{
	__super::Free();
}


