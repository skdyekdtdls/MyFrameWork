#include "..\Public\Cell.h"

#ifdef _DEBUG
#include "ColliderSphere.h"
#include "VIBuffer_Cell.h"
#endif

CCell::CCell(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: m_pDevice(pDevice)
	, m_pContext(pContext)
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

void CCell::Save(HANDLE hFile, DWORD& dwByte)
{
	WriteVoid(&m_iIndex, sizeof(_int));
	WriteVoid(&m_vPoints[0], sizeof(_float3) * POINT_END);
	WriteVoid(&m_iNeighborIndices[0], sizeof(_int) * NEIGHBOR_END);
}

void CCell::Load(HANDLE hFile, DWORD& dwByte, _uint iLevelIndex)
{
#ifdef _DEBUG
	Safe_Release(m_pVIBuffer);
#endif
	ReadVoid(&m_iIndex, sizeof(_int));
	ReadVoid(&m_vPoints[0], sizeof(_float3) * POINT_END);
	ReadVoid(&m_iNeighborIndices[0], sizeof(_int) * NEIGHBOR_END);
#ifdef _DEBUG
	..
	m_pVIBuffer = CVIBuffer_Cell::Create(m_pDevice, m_pContext, m_vPoints);
#endif
}

HRESULT CCell::Initialize(const _float3* pPoints, _int iIndex)
{
	if (nullptr != pPoints)
	{
		memcpy(m_vPoints, pPoints, sizeof(_float3) * POINT_END);
		m_iIndex = iIndex;
		_vector vLine;
		vLine = XMLoadFloat3(&m_vPoints[POINT_B]) - XMLoadFloat3(&m_vPoints[POINT_A]);
		m_vNormals[NEIGHBOR_AB] = _float3(XMVectorGetZ(vLine) * -1.f, 0.f, XMVectorGetX(vLine));

		vLine = XMLoadFloat3(&m_vPoints[POINT_C]) - XMLoadFloat3(&m_vPoints[POINT_B]);
		m_vNormals[NEIGHBOR_BC] = _float3(XMVectorGetZ(vLine) * -1.f, 0.f, XMVectorGetX(vLine));

		vLine = XMLoadFloat3(&m_vPoints[POINT_A]) - XMLoadFloat3(&m_vPoints[POINT_C]);
		m_vNormals[NEIGHBOR_CA] = _float3(XMVectorGetZ(vLine) * -1.f, 0.f, XMVectorGetX(vLine));
	}

#ifdef _DEBUG

//CColliderSphere::Clone();
//	m_pColliderSpheres.push_back();
//	m_pVIBuffer = CVIBuffer_Cell::Create(m_pDevice, m_pContext, pPoints);
	if (nullptr == m_pVIBuffer)
		return E_FAIL;
#endif

	return S_OK;
}

_bool CCell::Compare_Points(_fvector vSourPoint, _fvector vDestPoint)
{
	if (true == XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_A]), vSourPoint))
	{
		if (true == XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_B]), vDestPoint))
			return true;

		if (true == XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_C]), vDestPoint))
			return true;
	}

	if (true == XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_B]), vSourPoint))
	{
		if (true == XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_C]), vDestPoint))
			return true;

		if (true == XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_A]), vDestPoint))
			return true;
	}

	if (true == XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_C]), vSourPoint))
	{
		if (true == XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_A]), vDestPoint))
			return true;

		if (true == XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_B]), vDestPoint))
			return true;
	}

	return false;
}

_bool CCell::is_In(_fvector vPosition, _int* pNeighborIndex)
{
	for (size_t i = 0; i < NEIGHBOR_END; ++i)
	{
		_vector vDir = XMVector3Normalize(vPosition - XMLoadFloat3(&m_vPoints[i]));
		_vector vNormal = XMVector3Normalize(XMLoadFloat3(&m_vNormals[i]));

		if (0 < XMVectorGetX(XMVector3Dot(vDir, vNormal)))
		{
			*pNeighborIndex = m_iNeighborIndices[i];
			return false;
		}
	}

	return true;
}

#ifdef _DEBUG

HRESULT CCell::Render()
{
	NULL_CHECK_RETURN(m_pVIBuffer, E_FAIL);

	m_pVIBuffer->Render();

	return S_OK;
}

_bool CCell::PickedCell(const RAY& tRay)
{
	return _bool();
}

#endif

CCell* CCell::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _float3* pPoints, _int iIndex)
{
	CCell* pInstance = new CCell(pDevice, pContext);

	if (FAILED(pInstance->Initialize(pPoints, iIndex)))
	{
		MSG_BOX("Failed to Created CCell");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CCell::Free()
{
#ifdef _DEBUG
	Safe_Release(m_pVIBuffer);
#endif

	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
}