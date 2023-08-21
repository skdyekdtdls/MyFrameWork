#include "..\Public\Cell.h"
#include "GameInstance.h"
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
	// 오브젝트 풀로 보관하고 있다가?.
	// 파일을 읽어서 소환?
	
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

_float4 CCell::RandomPosition()
{
	_float4 vResult;
	_vector vRandomPoint = XMVectorZero();
	_vector vPointA = XMLoadFloat3(&m_vPoints[POINT_A]);
	_vector vPointB = XMLoadFloat3(&m_vPoints[POINT_B]);
	_vector vPointC = XMLoadFloat3(&m_vPoints[POINT_C]);
	_float weightU, weightV, weightW;
	weightU = RandomUNormal();
	weightV = RandomUNormal();

	if (weightU + weightV > 1)
	{
		weightU = 1 - weightU;
		weightV = 1 - weightV;
	}

	weightW = 1 - weightU - weightV;
	vRandomPoint = (weightU * vPointA) + (weightV * vPointB) + (weightW * vPointC);
	XMStoreFloat4(&vResult, vRandomPoint);
	vResult.w = 1.f;
	return vResult;
}

HRESULT CCell::Initialize(const _float3* pPoints, _int iIndex)
{
	if (nullptr != pPoints)
	{
		memcpy(m_vPoints, pPoints, sizeof(_float3) * POINT_END);
		m_iIndex = iIndex;
	}
#ifdef _DEBUG
	ClockWiseSort();
#endif

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

_bool CCell::is_In(_fvector vPosition)
{
	for (size_t i = 0; i < NEIGHBOR_END; ++i)
	{
		_vector vDir = XMVector3Normalize(vPosition - XMLoadFloat3(&m_vPoints[i]));
		_vector vNormal = XMVector3Normalize(XMLoadFloat3(&m_vNormals[i]));

		if (0 < XMVectorGetX(XMVector3Dot(vDir, vNormal)))
		{
			return false;
		}
	}

	return true;
}

void CCell::DecrementIndex()
{
	--m_iIndex;
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

HRESULT CCell::Render_CellIndex()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);
	
	// 중점 = 세 점의 위치 / 3
	_vector vCenter = (XMLoadFloat3(&m_vPoints[POINT_A]) + XMLoadFloat3(&m_vPoints[POINT_B]) + XMLoadFloat3(&m_vPoints[POINT_C])) / POINT_END;
	_vector vCamPos = pGameInstance->Get_CamPositionVector();
	_vector vDir;
	_float fRange = 40.f;
	_float fAtt;
	vDir = vCenter - vCamPos;
	_float fCenterLength;

	fCenterLength  = XMVectorGetX(XMVector3Length(vDir));
	fAtt = (fRange - fCenterLength) / fRange;
	if (fAtt < 0.f)// 0보다 작으면 어차피 출력이 안될것이므로 최적화를 위해 리턴함.
	{
		Safe_Release(pGameInstance);
		return S_OK; 
	}

	// 카메라가 멀어지면 폰트가 작아지고 카메라가 가까워지면 커지게한다.
	_matrix matWorld = XMMatrixIdentity();
	_matrix matView = pGameInstance->Get_TransformMatrix(CPipeLine::D3DTS_VIEW);
	_matrix matProj = pGameInstance->Get_TransformMatrix(CPipeLine::D3DTS_PROJ);
	_matrix matWVP = matWorld * matView * matProj;
	_float3 Center;
	XMStoreFloat3(&Center, XMVector3TransformCoord(vCenter, matWVP));

	_uint2 WinSize = pGameInstance->GetViewPortSize(m_pContext);
	_float2 FontPos = _float2();

	FontPos.x = (_float)WinSize.x * 0.5f * (Center.x + 1.0f);
	FontPos.y = (_float)WinSize.y * 0.5f * (1.0f - Center.y);

	fAtt = max(fAtt, 0.f);
	pGameInstance->Render_Font(TEXT("Font_135"), to_wstring(m_iIndex).c_str(), FontPos
		, XMVectorSet(1.f, 1.f, 1.f, 1.f), 0.f, _float2(0.f, 0.f) , 1.f * fAtt);

	Safe_Release(pGameInstance);
	
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
	//CA = XMLoadFloat3(&m_vPoints[POINT_A]) - XMLoadFloat3(&m_vPoints[POINT_C]);
	
	fResult = XMVectorGetY(XMVector3Cross(AB, BC));
	if (0 > fResult)
		std::swap(m_vPoints[POINT_A], m_vPoints[POINT_C]);

	//fResult = XMVectorGetY(XMVector3Cross(BC, CA));
	//if (0 > fResult)
	//	std::swap(m_vPoints[POINT_A], m_vPoints[POINT_C]);

	//fResult = XMVectorGetY(XMVector3Cross(CA, AB));
	//if (0 > fResult)
	//	std::swap(m_vPoints[POINT_A], m_vPoints[POINT_C]);
}

void CCell::Release_Debug()
{
	for (auto& Collider : m_pColliderSpheres)
		Safe_Release(Collider);
	m_pColliderSpheres.clear();
	Safe_Release(m_pVIBuffer);
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
	Release_Debug();
#endif
	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
}