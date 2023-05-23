#include "..\Public\VIBuffer_Grid.h"

CVIBuffer_Grid::CVIBuffer_Grid(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CVIBuffer(pDevice, pContext)
{
}

CVIBuffer_Grid::CVIBuffer_Grid(const CVIBuffer_Grid& rhs)
	: CVIBuffer(rhs)
{
}

HRESULT CVIBuffer_Grid::Initialize_Prototype(_float fWidth, _float fDepth, _uint iXNum, _uint iZNum)
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	_float halfWidth = 0.5f * fWidth;
	_float halfDepth = 0.5f * fDepth;
	_float dx = fWidth / (iXNum - 1);
	_float dz = fDepth / (iZNum - 1);
	_float du = 1.0f / (iXNum - 1);
	_float dv = 1.0f / (iZNum - 1);

	m_iNumVertexBuffers = { 1 };
	m_iStride = { sizeof VTXPOSTEX };
	m_iNumVertices = { iXNum * iZNum };
	m_iIndexStride = { sizeof _uint };
	m_iNumIndices = { (iXNum - 1) * (iZNum - 1) * 2 };
	m_eFormat = { DXGI_FORMAT_R32_UINT };
	m_eTopology = { D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST };

	ZeroStruct(m_BufferDesc);
	m_BufferDesc.ByteWidth = { m_iStride * m_iNumVertices };
	m_BufferDesc.Usage = { D3D11_USAGE_DEFAULT };
	m_BufferDesc.BindFlags = { D3D11_BIND_VERTEX_BUFFER };
	m_BufferDesc.CPUAccessFlags = { 0 };
	m_BufferDesc.MiscFlags = { 0 };
	m_BufferDesc.StructureByteStride = { m_iStride };

	VTXPOSTEX* pVertices = new VTXPOSTEX[m_iNumVertices];
	_uint* pIndices = new _uint[m_iNumIndices];

	for (_uint i = 0; i < iZNum; ++i)
	{
		_float z = halfDepth - i * dz;
		for (_uint j = 0; j < iXNum; ++j)
		{
			_float x = -halfWidth + j * dx;
			pVertices[i * iXNum + j].vPosition = _float3(x, 0.0f, z);
			pVertices[i * iXNum + j].vTexCoord.x = j * du;
			pVertices[i * iXNum + j].vTexCoord.y = j * dv;
		}
	}

	ZeroStruct(m_SubResourceData);
	m_SubResourceData.pSysMem = pVertices;
	if (FAILED(m_pDevice->CreateBuffer(&m_BufferDesc, &m_SubResourceData, &m_pVB)))
	{
		assert(false);
		return E_FAIL;
	}

	ZeroStruct(m_BufferDesc);
	m_BufferDesc.ByteWidth = { m_iIndexStride * m_iNumIndices };
	m_BufferDesc.Usage = { D3D11_USAGE_DEFAULT };
	m_BufferDesc.BindFlags = { D3D11_BIND_INDEX_BUFFER };
	m_BufferDesc.CPUAccessFlags = { 0 };
	m_BufferDesc.MiscFlags = { 0 };
	m_BufferDesc.StructureByteStride = { m_iIndexStride };

	_uint k = 0;
	for (_uint i = 0; i < iZNum - 1; ++i)
	{
		for (_uint j = 0; j < iXNum - 1; ++j)
		{
			pIndices[k]		= i * iXNum + j;
			pIndices[k + 1] = i * iXNum + j + 1;
			pIndices[k + 2] = (i + 1) * iXNum + j;

			pIndices[k + 3] = (i + 1) * iXNum + j;
			pIndices[k + 4] = i * iXNum + j + 1;
			pIndices[k + 5] = (i + 1) * iXNum + j + 1;

			k += 6; // next quad
		}
	}

	ZeroStruct(m_SubResourceData);
	m_SubResourceData.pSysMem = pIndices;
	if (FAILED(m_pDevice->CreateBuffer(&m_BufferDesc, &m_SubResourceData, &m_pIB)))
	{
		assert(false);
		return E_FAIL;
	}

	Safe_Delete_Array(pIndices);
	Safe_Delete_Array(pVertices);
	return S_OK;
}

HRESULT CVIBuffer_Grid::Initialize(void* pArg)
{
	return S_OK;
}

CVIBuffer_Grid* CVIBuffer_Grid::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext
	, _float fWidth, _float fDepth, _uint iXNum, _uint iZNum)
{
	CVIBuffer_Grid* pInstance = new CVIBuffer_Grid(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(fWidth, fDepth, iXNum, iZNum)))
	{
		MSG_BOX("Failed to Created CVIBuffer_Grid");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CComponent* CVIBuffer_Grid::Clone(void* pArg)
{
	CVIBuffer_Grid* pInstance = new CVIBuffer_Grid(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned CVIBuffer_Grid");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CVIBuffer_Grid::Free(void)
{
	__super::Free();
}
