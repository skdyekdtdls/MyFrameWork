#include "..\Public\Mesh.h"
#include "Bone.h"
CMesh::CMesh(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CVIBuffer(pDevice, pContext)
{
}

CMesh::CMesh(const CMesh& rhs)
	: CVIBuffer(rhs)
{
}

void CMesh::Get_Matrices(CModel::BONES Bones, _float4x4* pMatrices, _fmatrix PivotMatrix)
{
	_uint	iIndex = 0;

	for (auto iBoneIndex : m_BoneIndices)
	{
		_float4x4 OffsetMatrix = Bones[iBoneIndex]->Get_OffsetMatrix();
		_float4x4 CombinedTransformationMatrix = Bones[iBoneIndex]->Get_CombinedTransformationMatrix();
		XMStoreFloat4x4(&pMatrices[iIndex++],
			XMLoadFloat4x4(&OffsetMatrix) *
			XMLoadFloat4x4(&CombinedTransformationMatrix) *
			PivotMatrix);
	}
}

HRESULT CMesh::Initialize_Prototype(CModel::TYPE eType, const CModel::BONES& Bones, const MESH* pMesh, _fmatrix PivotMatrix)
{
	m_iMaterialIndex = pMesh->m_MaterialIndex;
	strcpy_s(m_szName, pMesh->m_Name.m_data);
	m_iNumVertexBuffers = { 1 };

	m_iNumVertices = { pMesh->m_NumVertices };
	m_iIndexStride = { sizeof(_ulong) };
	m_iNumIndices = { pMesh->m_NumFaces * 3 };
	m_eFormat = { DXGI_FORMAT_R32_UINT };
	m_eTopology = { D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST };

	HRESULT hr = { 0 };

	if (CModel::TYPE_NONANIM == eType)
		hr = Ready_VertexBuffer_NonAnim(pMesh, PivotMatrix);
	else
		hr = Ready_VertexBuffer_Anim(pMesh, Bones);
	if (FAILED(hr))
		return E_FAIL;

	ZeroStruct(m_BufferDesc);

	m_BufferDesc.ByteWidth = { m_iIndexStride * m_iNumIndices };
	m_BufferDesc.Usage = { D3D11_USAGE_DEFAULT };
	m_BufferDesc.BindFlags = { D3D11_BIND_INDEX_BUFFER };
	m_BufferDesc.StructureByteStride = { 0 };
	m_BufferDesc.CPUAccessFlags = { 0 };
	m_BufferDesc.MiscFlags = { 0 };
	
	_uint iNumFaces = { 0 };

	_ulong* pIndices = new _ulong[m_iNumIndices];

	for (size_t i = 0; i < pMesh->m_NumFaces; ++i)
	{
		pIndices[iNumFaces++] = pMesh->m_Faces[i].m_Indices[0];
		pIndices[iNumFaces++] = pMesh->m_Faces[i].m_Indices[1];
		pIndices[iNumFaces++] = pMesh->m_Faces[i].m_Indices[2];
	}

	ZeroStruct(m_SubResourceData);
	m_SubResourceData.pSysMem = { pIndices };

	FAILED_CHECK_RETURN(__super::Create_Buffer(&m_pIB), E_FAIL);

	Safe_Delete_Array(pIndices);

	return S_OK;
}

HRESULT CMesh::Initialize(void* pArg)
{
	return S_OK;
}

HRESULT CMesh::Ready_VertexBuffer_NonAnim(const MESH* pMesh, _matrix PivotMatrix)
{
	m_iStride = { sizeof(VTXMESH) };

	ZeroStruct(m_BufferDesc);

	m_BufferDesc.ByteWidth = { m_iStride * m_iNumVertices };
	m_BufferDesc.Usage = { D3D11_USAGE_DEFAULT };
	m_BufferDesc.BindFlags = { D3D11_BIND_VERTEX_BUFFER };
	m_BufferDesc.StructureByteStride = { m_iStride };
	m_BufferDesc.CPUAccessFlags = { 0 };
	m_BufferDesc.MiscFlags = { 0 };

	VTXMESH* pVertices = new VTXMESH[m_iNumVertices];
	ZeroMemory(pVertices, sizeof(VTXMESH) * m_iNumVertices);

	for (size_t i = 0; i < m_iNumVertices; ++i)
	{
		memcpy(&pVertices[i].vPosition, &pMesh->m_Vertices[i], sizeof(_float3));
		XMStoreFloat3(&pVertices[i].vPosition,
			XMVector3TransformCoord(XMLoadFloat3(&pVertices[i].vPosition), PivotMatrix));

		memcpy(&pVertices[i].vNormal, &pMesh->m_Normals[i], sizeof(_float3));
		XMStoreFloat3(&pVertices[i].vNormal,
			XMVector3TransformCoord(XMLoadFloat3(&pVertices[i].vNormal), PivotMatrix));

		memcpy(&pVertices[i].vTexCoord, &pMesh->m_TextureCoords[i], sizeof(_float2));

		memcpy(&pVertices[i].vTangent, &pMesh->m_Tangents[i], sizeof(_float3));
	}

	ZeroStruct(m_SubResourceData);
	m_SubResourceData.pSysMem = pVertices;

	FAILED_CHECK_RETURN(__super::Create_Buffer(&m_pVB), E_FAIL);

	return S_OK;
}

HRESULT CMesh::Ready_VertexBuffer_Anim(const MESH* pMesh, const CModel::BONES& Bones)
{
	m_iStride = { sizeof(VTXANIMMESH) };

	m_BufferDesc.ByteWidth = { m_iStride * m_iNumVertices };
	m_BufferDesc.Usage = { D3D11_USAGE_DEFAULT };
	m_BufferDesc.BindFlags = { D3D11_BIND_VERTEX_BUFFER };
	m_BufferDesc.StructureByteStride = { m_iStride };
	m_BufferDesc.CPUAccessFlags = { 0 };
	m_BufferDesc.MiscFlags = { 0 };

	VTXANIMMESH* pVertices = new VTXANIMMESH[m_iNumVertices];
	ZeroStruct(m_BufferDesc);

	for (size_t i = 0; i < m_iNumVertices; ++i)
	{
		memcpy(&pVertices[i].vPosition, &pMesh->m_Vertices[i], sizeof(_float3));
		memcpy(&pVertices[i].vNormal, &pMesh->m_Normals[i], sizeof(_float3));
		memcpy(&pVertices[i].vTexCoord, &pMesh->m_TextureCoords[i], sizeof(_float2));
		memcpy(&pVertices[i].vTangent, &pMesh->m_Tangents[i], sizeof(_float3));
	}

	m_iNumBones = pMesh->m_NumBones;

	for (size_t i = 0; i < pMesh->m_NumBones; ++i)
	{
		BONE* pBone = &pMesh->m_Bones[i];

		_uint	iBoneIndex = 0;

		auto	iter = find_if(Bones.begin(), Bones.end(), [&](CBone* pValue)
			{
				if (0 != strcmp(pBone->m_Name.m_data, pValue->Get_Name()))
				{
					++iBoneIndex;
					return false;
				}
				else
					return true;
			});

		_float4x4	OffsetMatrix;
		memcpy(&OffsetMatrix, &pBone->m_OffsetMatrix, sizeof(_float4x4));
		Bones[iBoneIndex]->Set_OffsetMatrix(OffsetMatrix);

		m_BoneIndices.push_back(iBoneIndex);

		for (size_t j = 0; j < pBone->m_NumWeights; ++j)
		{
			VERTEX_WEIGHT VertexWeight = pBone->m_Weights[i];

			if (0.0f == pVertices[VertexWeight.m_VertexId].vBlendWeights.x)
			{
				pVertices[VertexWeight.m_VertexId].vBlendIndices.x = i;
				pVertices[VertexWeight.m_VertexId].vBlendWeights.x = VertexWeight.m_Weight;
			}
			else if (0.0f == pVertices[VertexWeight.m_VertexId].vBlendWeights.y)
			{
				pVertices[VertexWeight.m_VertexId].vBlendIndices.y = i;
				pVertices[VertexWeight.m_VertexId].vBlendWeights.y = VertexWeight.m_Weight;
			}
			else if (0.0f == pVertices[VertexWeight.m_VertexId].vBlendWeights.z)
			{
				pVertices[VertexWeight.m_VertexId].vBlendIndices.z = i;
				pVertices[VertexWeight.m_VertexId].vBlendWeights.z = VertexWeight.m_Weight;
			}
			else if (0.0f == pVertices[VertexWeight.m_VertexId].vBlendWeights.w)
			{
				pVertices[VertexWeight.m_VertexId].vBlendIndices.w = i;
				pVertices[VertexWeight.m_VertexId].vBlendWeights.w = VertexWeight.m_Weight;
			}
		}
	}

	// »À¸¦ ¸¸µé¾îÁÜ?
	if (0 == m_iNumBones)
	{
		m_iNumBones = 1;

		_uint iBoneIndex = 0;

		auto	iter = find_if(Bones.begin(), Bones.end(), [&](CBone* pValue)
			{
				if (0 != strcmp(m_szName, pValue->Get_Name()))
				{
					++iBoneIndex;
					return false;
				}
				else
					return true;
			});

		m_BoneIndices.push_back(iBoneIndex);
	}

	ZeroMemory(&m_SubResourceData, sizeof m_SubResourceData);
	m_SubResourceData.pSysMem = pVertices;

	if (FAILED(__super::Create_Buffer(&m_pVB)))
		return E_FAIL;
	Safe_Delete_Array(pVertices);

	return S_OK;
}

CMesh* CMesh::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CModel::TYPE eType, const CModel::BONES& Bones, const MESH* pMesh, _fmatrix PivotMatrix)
{
	CMesh* pInstance = new CMesh(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(eType, Bones, pMesh, PivotMatrix)))
	{
		MSG_BOX("Failed to Created CMesh");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CMesh::Clone(void* pArg)
{
	CMesh* pInstance = new CMesh(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned CMesh");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CMesh::Free()
{
	__super::Free();
}
