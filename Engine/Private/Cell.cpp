#include "..\Public\Cell.h"

#ifdef _DEBUG
#include "VIBuffer_Cell.h"
#include "ColliderSphere.h"
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
	WriteVoid(&m_vPoints[0], sizeof(_float3) * POINT_END);
}

void CCell::Set_Point(POINT ePoint, const _float3* vPos)
{
	m_vPoints[ePoint] = *vPos;
	CalcNormal();

#ifdef _DEBUG
	ID3D11Buffer* pVB = m_pVIBuffer->GetVB();
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	ZeroStruct(mappedResource);

	// VIBuffer업데이트
	m_pContext->Map(pVB, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &mappedResource);
	VTXPOS* pVertices = reinterpret_cast<VTXPOS*>(mappedResource.pData);
	pVertices[ePoint].vPosition = *vPos;
	m_pContext->Unmap(pVB, 0);

	// 콜라이더 위치 업데이트
	_float4x4 TransformationFloat4x4;
	XMStoreFloat4x4(&TransformationFloat4x4, XMMatrixIdentity());
	memcpy(&TransformationFloat4x4.m[3][0], &m_vPoints[ePoint], sizeof(_float3));
	m_pColliderSpheres[ePoint]->Tick(XMLoadFloat4x4(&TransformationFloat4x4));
#endif
}

void CCell::CalcNormal()
{
	_vector vLine;
	vLine = XMLoadFloat3(&m_vPoints[POINT_B]) - XMLoadFloat3(&m_vPoints[POINT_A]);
	m_vNormals[NEIGHBOR_AB] = _float3(XMVectorGetZ(vLine) * -1.f, 0.f, XMVectorGetX(vLine));

	vLine = XMLoadFloat3(&m_vPoints[POINT_C]) - XMLoadFloat3(&m_vPoints[POINT_B]);
	m_vNormals[NEIGHBOR_BC] = _float3(XMVectorGetZ(vLine) * -1.f, 0.f, XMVectorGetX(vLine));

	vLine = XMLoadFloat3(&m_vPoints[POINT_A]) - XMLoadFloat3(&m_vPoints[POINT_C]);
	m_vNormals[NEIGHBOR_CA] = _float3(XMVectorGetZ(vLine) * -1.f, 0.f, XMVectorGetX(vLine));
}

HRESULT CCell::Initialize(const _float3* pPoints, _int iIndex)
{
	if (nullptr != pPoints)
	{
		memcpy(m_vPoints, pPoints, sizeof(_float3) * POINT_END);
		m_iIndex = iIndex;
	}

	ClockWiseSort();
	CalcNormal();
#ifdef _DEBUG
	for (size_t i = 0; i < POINT_END; ++i)
		m_pColliderSpheres.push_back(CColliderSphere::Create(m_pDevice, m_pContext));
	UpdateColliderForVertices();
	m_pVIBuffer = CVIBuffer_Cell::Create(m_pDevice, m_pContext, pPoints);
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

_bool CCell::is_In(_fvector vPosition, _int* pNeighborIndex, NEIGHBOR& eNeighbor)
{
	for (size_t i = 0; i < NEIGHBOR_END; ++i)
	{
		_vector vDir = XMVector3Normalize(vPosition - XMLoadFloat3(&m_vPoints[i]));
		_vector vNormal = XMVector3Normalize(XMLoadFloat3(&m_vNormals[i]));

		if (0 < XMVectorGetX(XMVector3Dot(vDir, vNormal)))
		{
			eNeighbor = static_cast<NEIGHBOR>(i);
			*pNeighborIndex = m_iNeighborIndices[i];
			return false;
		}
	}

	return true;
}

#ifdef _DEBUG
HRESULT CCell::Render_ColliderSphere()
{
	for (auto* Collider : m_pColliderSpheres)
	{
		Collider->Render();
	}

	return S_OK;
}

HRESULT CCell::Render_VIBuffer()
{
	NULL_CHECK_RETURN(m_pVIBuffer, E_FAIL);

	m_pVIBuffer->Render();

	return S_OK;
}

void CCell::UpdateColliderForVertex(POINT ePoint)
{
	_float4x4 TransformationFloat4x4;
	XMStoreFloat4x4(&TransformationFloat4x4, XMMatrixIdentity());
	memcpy(&TransformationFloat4x4.m[3][0], &m_vPoints[ePoint], sizeof(_float3));
	m_pColliderSpheres[ePoint]->Tick(XMLoadFloat4x4(&TransformationFloat4x4));
}

void CCell::UpdateColliderForVertices()
{
	for (size_t i = 0; i < POINT_END; ++i)
	{
		_float4x4 TransformationFloat4x4;
		XMStoreFloat4x4(&TransformationFloat4x4, XMMatrixIdentity());
		memcpy(&TransformationFloat4x4.m[3][0], &m_vPoints[i], sizeof(_float3));
		m_pColliderSpheres[i]->Tick(XMLoadFloat4x4(&TransformationFloat4x4));
	}
}

_bool CCell::IsCellVertexPicked(CELL_PICK_DESC& tPickDesc, const RAY& tRay)
{
	for (size_t i = 0; i < POINT_END; ++i)
	{
		_float fDistance = { FLT_MAX };
		if (m_pColliderSpheres[i]->IntersectRay(fDistance, tRay))
		{
			tPickDesc.vPickPos = *(_float4*)&m_vPoints[i];
			tPickDesc.fDist = fDistance;
			tPickDesc.pPickedCell = this;
			tPickDesc.iVertexIndex = { (_int)i };
			return true;
		}
	}

	return false;
}

_bool CCell::IsCellPicked(CELL_PICK_DESC& tPickDesc, const RAY& tRay)
{
	return _bool();
}

void CCell::ClockWiseSort()
{
	// CA cross AB = 음수면 정렬
	_float fResult;
	_vector AB, BC, CA;
	AB = XMLoadFloat3(&m_vPoints[POINT_B]) - XMLoadFloat3(&m_vPoints[POINT_A]);
	BC = XMLoadFloat3(&m_vPoints[POINT_C]) - XMLoadFloat3(&m_vPoints[POINT_B]);
	CA = XMLoadFloat3(&m_vPoints[POINT_A]) - XMLoadFloat3(&m_vPoints[POINT_C]);
	
	fResult = XMVectorGetY(XMVector3Cross(AB, BC));
	if (fResult < 0)
		std::swap(m_vPoints[POINT_A], m_vPoints[POINT_C]);

	fResult = XMVectorGetY(XMVector3Cross(BC, CA));
	if (fResult < 0)
		std::swap(m_vPoints[POINT_A], m_vPoints[POINT_C]);

	fResult = XMVectorGetY(XMVector3Cross(CA, AB));
	if (fResult < 0)
		std::swap(m_vPoints[POINT_A], m_vPoints[POINT_C]);
}

#endif

void CCell::Release_Debug()
{
	for (auto& Collider : m_pColliderSpheres)
		Safe_Release(Collider);
	m_pColliderSpheres.clear();
	Safe_Release(m_pVIBuffer);
}

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
	Release_Debug();
#endif
	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
}