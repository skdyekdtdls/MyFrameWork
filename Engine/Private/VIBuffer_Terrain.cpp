#include "..\Public\VIBuffer_Terrain.h"

CVIBuffer_Terrain::CVIBuffer_Terrain(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CVIBuffer(pDevice, pContext)
{
}

CVIBuffer_Terrain::CVIBuffer_Terrain(const CVIBuffer_Terrain& rhs)
	: CVIBuffer(rhs)
	, m_pVertices(rhs.m_pVertices)
	, m_iNumVerticesX(rhs.m_iNumVerticesX)
	, m_iNumVerticesZ(rhs.m_iNumVerticesZ)
{
	
}

HRESULT CVIBuffer_Terrain::Initialize_Prototype(const _tchar* pHeightMap) // 지금 여기서 추가하고있잖아. 그냥 함수하나파서
{
	_ulong			dwByte = { 0 };
	HANDLE			hFile = CreateFile(pHeightMap, GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

	BITMAPFILEHEADER	fh;
	BITMAPINFOHEADER	ih;

	ReadFile(hFile, &fh, sizeof fh, &dwByte, nullptr);
	ReadFile(hFile, &ih, sizeof ih, &dwByte, nullptr);

	m_iNumVerticesX = { (_uint)ih.biWidth };
	m_iNumVerticesZ = { (_uint)ih.biHeight };
	
	_ulong* pPixel = new _ulong[m_iNumVerticesX * m_iNumVerticesZ];
	ZeroMemory(pPixel, sizeof(_ulong) * m_iNumVerticesX * m_iNumVerticesZ);

	ReadFile(hFile, pPixel, sizeof(_ulong) * m_iNumVerticesX * m_iNumVerticesZ, &dwByte, nullptr);

	CloseHandle(hFile);


	m_iNumVertexBuffers = { 1 };
	m_iStride = { sizeof(VTXPOSNORTEX) };
	m_iNumVertices = { m_iNumVerticesX * m_iNumVerticesZ };
	m_iIndexStride = { sizeof(_ulong) };
	m_iNumIndices = { (m_iNumVerticesX - 1) * (m_iNumVerticesZ - 1) * 2 * 3 };
	m_eFormat = { DXGI_FORMAT_R32_UINT };
	m_eTopology = { D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST };


#pragma region VERTEX_BUFFER

	//	11111111 01011101 01011101 01011101
	//&	00000000 00000000 00000000 11111111]
	//
	//
	//
	//	00000000 00000000 00000000 01011101



	m_pVertices = new VTXPOSNORTEX[m_iNumVertices];
	ZeroMemory(m_pVertices, sizeof(VTXPOSNORTEX) * m_iNumVertices);

	for (_uint i = 0; i < m_iNumVerticesZ; ++i)
	{
		for (_uint j = 0; j < m_iNumVerticesX; j++)
		{
			_uint		iIndex = i * m_iNumVerticesX + j;

			m_pVertices[iIndex].vPosition = _float3(j, 0.f /*(pPixel[iIndex] & 0x000000ff) / 10.0f */, i);
			m_pVertices[iIndex].vNormal = _float3(0.f, 0.f, 0.f);
			m_pVertices[iIndex].vTexCoord = _float2(j / (m_iNumVerticesX - 1.f), i / (m_iNumVerticesZ - 1.f));
		}
	}

	Safe_Delete_Array(pPixel);

#pragma endregion


#pragma region INDEX_BUFFER

	_ulong* pIndices = new _ulong[m_iNumIndices];
	ZeroMemory(pIndices, sizeof(_ulong) * m_iNumIndices);

	_uint		iNumIndices = { 0 };

	for (_uint i = 0; i < m_iNumVerticesZ - 1; ++i)
	{
		for (_uint j = 0; j < m_iNumVerticesX - 1; j++)
		{
			_uint		iIndex = i * m_iNumVerticesX + j;

			_uint		iIndices[4] = {
				iIndex + m_iNumVerticesX,
				iIndex + m_iNumVerticesX + 1,
				iIndex + 1,
				iIndex
			};

			_vector		vSour, vDest, vNormal;

			pIndices[iNumIndices++] = iIndices[0];
			pIndices[iNumIndices++] = iIndices[1];
			pIndices[iNumIndices++] = iIndices[2];

			vSour = XMLoadFloat3(&m_pVertices[iIndices[1]].vPosition) -
				XMLoadFloat3(&m_pVertices[iIndices[0]].vPosition);
			vDest = XMLoadFloat3(&m_pVertices[iIndices[2]].vPosition) -
				XMLoadFloat3(&m_pVertices[iIndices[1]].vPosition);
			vNormal = XMVector3Normalize(XMVector3Cross(vSour, vDest));


			XMStoreFloat3(&m_pVertices[iIndices[0]].vNormal,
				XMLoadFloat3(&m_pVertices[iIndices[0]].vNormal) + vNormal);
			XMStoreFloat3(&m_pVertices[iIndices[1]].vNormal,
				XMLoadFloat3(&m_pVertices[iIndices[1]].vNormal) + vNormal);
			XMStoreFloat3(&m_pVertices[iIndices[2]].vNormal,
				XMLoadFloat3(&m_pVertices[iIndices[2]].vNormal) + vNormal);

			pIndices[iNumIndices++] = iIndices[0];
			pIndices[iNumIndices++] = iIndices[2];
			pIndices[iNumIndices++] = iIndices[3];

			vSour = XMLoadFloat3(&m_pVertices[iIndices[2]].vPosition) -
				XMLoadFloat3(&m_pVertices[iIndices[0]].vPosition);
			vDest = XMLoadFloat3(&m_pVertices[iIndices[3]].vPosition) -
				XMLoadFloat3(&m_pVertices[iIndices[2]].vPosition);
			vNormal = XMVector3Normalize(XMVector3Cross(vSour, vDest));

			XMStoreFloat3(&m_pVertices[iIndices[0]].vNormal,
				XMLoadFloat3(&m_pVertices[iIndices[0]].vNormal) + vNormal);
			XMStoreFloat3(&m_pVertices[iIndices[2]].vNormal,
				XMLoadFloat3(&m_pVertices[iIndices[2]].vNormal) + vNormal);
			XMStoreFloat3(&m_pVertices[iIndices[3]].vNormal,
				XMLoadFloat3(&m_pVertices[iIndices[3]].vNormal) + vNormal);
		}
	}

	for (size_t i = 0; i < m_iNumVertices; i++)
	{
		XMStoreFloat3(&m_pVertices[i].vNormal,
			XMVector3Normalize(XMLoadFloat3(&m_pVertices[i].vNormal)));
	}

	ZeroMemory(&m_BufferDesc, sizeof m_BufferDesc);

	m_BufferDesc.ByteWidth = { m_iStride * m_iNumVertices };
	m_BufferDesc.Usage = { D3D11_USAGE_DEFAULT };
	m_BufferDesc.BindFlags = { D3D11_BIND_VERTEX_BUFFER };
	m_BufferDesc.StructureByteStride = { m_iStride };
	m_BufferDesc.CPUAccessFlags = { 0 };
	m_BufferDesc.MiscFlags = { 0 };

	ZeroMemory(&m_SubResourceData, sizeof m_SubResourceData);
	m_SubResourceData.pSysMem = m_pVertices;

	if (FAILED(__super::Create_Buffer(&m_pVB)))
		return E_FAIL;

	ZeroMemory(&m_BufferDesc, sizeof m_BufferDesc);

	m_BufferDesc.ByteWidth = { m_iIndexStride * m_iNumIndices };
	m_BufferDesc.Usage = { D3D11_USAGE_DEFAULT };
	m_BufferDesc.BindFlags = { D3D11_BIND_INDEX_BUFFER };
	m_BufferDesc.StructureByteStride = { 0 };
	m_BufferDesc.CPUAccessFlags = { 0 };
	m_BufferDesc.MiscFlags = { 0 };

	ZeroMemory(&m_SubResourceData, sizeof m_SubResourceData);
	m_SubResourceData.pSysMem = pIndices;

	if (FAILED(__super::Create_Buffer(&m_pIB)))
		return E_FAIL;

	Safe_Delete_Array(pIndices);

#pragma endregion

	return S_OK;
}

HRESULT CVIBuffer_Terrain::Initialize(void* pArg)
{
	return S_OK;
}

_bool CVIBuffer_Terrain::IsPicked(FXMVECTOR vRayOrigin, FXMVECTOR vRayDir, _float& fMinDist)
{
	_bool bResult = { false };
	for (_ulong i = 0; i < m_iNumVerticesZ -1; ++i)
	{
		for (_ulong j = 0; j < m_iNumVerticesX - 1; ++j)
		{
			_uint iIndex = i * m_iNumVerticesX + j;

			//if (iIndex >= (m_iNumVerticesX - 1)* (m_iNumVerticesZ - 1))
			//	return false;

			_float fDist;
			_uint		iIndices[4] = {
				iIndex + m_iNumVerticesX,
				iIndex + m_iNumVerticesX + 1,
				iIndex + 1,
				iIndex
			};
			if (TriangleTests::Intersects(
				vRayOrigin
				, vRayDir
				, XMLoadFloat3(&m_pVertices[iIndices[0]].vPosition)
				, XMLoadFloat3(&m_pVertices[iIndices[1]].vPosition)
				, XMLoadFloat3(&m_pVertices[iIndices[2]].vPosition)
				, fDist))
			{
				bResult = { true };
				if (fDist < fMinDist)
					fMinDist = fDist;				
			}

			if (TriangleTests::Intersects(
				vRayOrigin
				, vRayDir
				, XMLoadFloat3(&m_pVertices[iIndices[0]].vPosition)
				, XMLoadFloat3(&m_pVertices[iIndices[2]].vPosition)
				, XMLoadFloat3(&m_pVertices[iIndices[3]].vPosition)
				, fDist))
			{

				bResult = { true };
				if (fDist < fMinDist)
					fMinDist = fDist;
			}
		}
	}
	
	system("cls");
	return bResult;
}

CVIBuffer_Terrain* CVIBuffer_Terrain::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _tchar* pHeightMap)
{
	CVIBuffer_Terrain* pInstance = new CVIBuffer_Terrain(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(pHeightMap)))
	{
		MSG_BOX("Failed to Created CVIBuffer_Terrain");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CComponent* CVIBuffer_Terrain::Clone(void* pArg)
{
	CVIBuffer_Terrain* pInstance = new CVIBuffer_Terrain(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned CVIBuffer_Terrain");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CVIBuffer_Terrain::Free()
{
	__super::Free();
	
	if(!m_IsCloned)
		Safe_Delete_Array(m_pVertices);
}