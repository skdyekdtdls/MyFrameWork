#ifdef _USE_IMGUI
#include "ImVertex.h"

CImVertex::CImVertex(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CVIBuffer(pDevice, pContext)
{
}

CImVertex::CImVertex(const CImVertex& rhs)
	: CVIBuffer(rhs)
{

}

HRESULT CImVertex::Initialize_Prototype()
{
	_float radius = 1;
	_uint sliceCount = 3;
	_uint stackCount = 3;
	VTXPOSCOL topVertex = { _float3(0.0f, +radius, 0.0f), _float4(0.0f, 0.0f, 0.f, 1.f) };
	VTXPOSCOL bottomVertex = { _float3(0.0f, -radius, 0.0f), _float4(0.0f, 0.0f, 0.f, 1.f) };

	std::vector<VTXPOSCOL> vertices;
	std::vector<WORD> indices;
	vertices.push_back(topVertex);

	float phiStep = XM_PI / stackCount;
	float thetaStep = 2.0f * XM_PI / sliceCount;

	for (_uint i = 1; i <= stackCount - 1; ++i)
	{
		float phi = i * phiStep;
		for (_uint j = 0; j <= sliceCount; ++j)
		{
			float theta = j * thetaStep;

			VTXPOSCOL v;

			v.vPosition.x = radius * sinf(phi) * cosf(theta);
			v.vPosition.y = radius * cosf(phi);
			v.vPosition.z = radius * sinf(phi) * sinf(theta);

			v.vPosition.x = theta / XM_2PI;
			v.vPosition.y = phi / XM_PI;

			vertices.push_back(v);
		}
	}
	vertices.push_back(bottomVertex);

	for (_uint i = 1; i <= sliceCount; ++i)
	{
		indices.push_back(0);
		indices.push_back(i + 1);
		indices.push_back(i);
	}

	_uint baseIndex = 1;
	_uint ringVertexCount = sliceCount + 1;

	for (_uint i = 0; i < stackCount - 2; ++i)
	{
		for (_uint j = 0; j < sliceCount; ++j)
		{
			indices.push_back(baseIndex + i * ringVertexCount + j);
			indices.push_back(baseIndex + i * ringVertexCount + j + 1);
			indices.push_back(baseIndex + (i + 1) * ringVertexCount + j);

			indices.push_back(baseIndex + (i + 1) * ringVertexCount + j);
			indices.push_back(baseIndex + i * ringVertexCount + j + 1);
			indices.push_back(baseIndex + (i + 1) * ringVertexCount + j + 1);
		}
	}

	_uint southPoleIndex = (_uint)vertices.size() - 1;

	baseIndex = southPoleIndex - ringVertexCount;

	for (_uint i = 0; i < sliceCount; ++i)
	{
		indices.push_back(southPoleIndex);
		indices.push_back(baseIndex + i);
		indices.push_back(baseIndex + i + 1);
	}

	m_iNumVertexBuffers = { 1 };
	m_iStride = { sizeof(VTXPOSCOL) };
	m_iNumVertices = { (_uint)vertices.size() };
	m_iIndexStride = { sizeof _ushort };
	m_iNumIndices = { (_uint)indices.size() };
	m_eFormat = { DXGI_FORMAT_R16_UINT };
	m_eTopology = { D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST };

	ZeroMemory(&m_BufferDesc, sizeof m_BufferDesc);
	m_BufferDesc.ByteWidth = { m_iStride * m_iNumVertices };
	m_BufferDesc.Usage = { D3D11_USAGE_DEFAULT };
	m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_BufferDesc.StructureByteStride = { m_iStride };
	m_BufferDesc.CPUAccessFlags = { 0 };
	m_BufferDesc.MiscFlags = { 0 };

	ZeroMemory(&m_SubResourceData, sizeof m_SubResourceData);
	m_SubResourceData.pSysMem = { &vertices };

	if (FAILED(__super::Create_Buffer(&m_pVB)))
		return E_FAIL;

	ZeroMemory(&m_BufferDesc, sizeof m_BufferDesc);
	m_BufferDesc.ByteWidth = { m_iIndexStride * m_iNumIndices };
	m_BufferDesc.Usage = { D3D11_USAGE_DEFAULT };
	m_BufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	m_BufferDesc.StructureByteStride = { 0 };
	m_BufferDesc.CPUAccessFlags = { 0 };
	m_BufferDesc.MiscFlags = { 0 };

	ZeroMemory(&m_SubResourceData, sizeof D3D11_SUBRESOURCE_DATA);
	m_SubResourceData.pSysMem = { &indices };

	if (FAILED(__super::Create_Buffer(&m_pIB)))
		return E_FAIL;

}

HRESULT CImVertex::Initialize(void* pArg)
{
	return S_OK;
}


CImVertex* CImVertex::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CImVertex* pInstance = new CImVertex(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created CImVertex");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CImVertex::Clone(void* pArg)
{
	CImVertex* pInstance = new CImVertex(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned CImVertex");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CImVertex::Free()
{
	__super::Free();
}
#endif