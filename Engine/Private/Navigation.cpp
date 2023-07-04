#include "Navigation.h"
#include "Cell.h"
#include "Shader.h"
#include "PipeLine.h"

CNavigation::CNavigation(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CComponent(pDevice, pContext)
{
}

CNavigation::CNavigation(const CNavigation& rhs)
	: CComponent(rhs)
	, m_Cells(rhs.m_Cells)
#ifdef _DEBUG
	, m_pShader(rhs.m_pShader)
#endif // _DEBUG
{
	for (auto& pCell : m_Cells)
		Safe_AddRef(pCell);
#ifdef _DEBUG
	Safe_AddRef(m_pShader);
#endif // _DEBUG
}

void CNavigation::Save(HANDLE hFile, DWORD& dwByte)
{
	for (auto& Cell : m_Cells)
		Cell->Save(hFile, dwByte);
}

HRESULT CNavigation::Initialize_Prototype(const _tchar* pNavigationDataFiles)
{
	if (nullptr != pNavigationDataFiles)
	{
		_ulong dwByte = { 0 };

		HANDLE hFile = CreateFile(pNavigationDataFiles, GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
		if (0 == hFile)
			return E_FAIL;

		_float3 vPoints[CCell::POINT_END];

		while (true)
		{
			ReadVoid(vPoints, sizeof(_float3) * CCell::POINT_END);

			if (0 == dwByte)
				break;

			AddCell(vPoints);
		}


		CloseHandle(hFile);
	}

	SetUp_Neighbors();

#ifdef _DEBUG
	UpdateCellsCollider();
	m_pShader = CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_Navigation.hlsl"), VTXPOS_DECL::Elements, VTXPOS_DECL::iNumElements);
	NULL_CHECK_RETURN(m_pShader, E_FAIL);
#endif // _DEBUG

	return S_OK;
}

HRESULT CNavigation::Initialize(void* pArg)
{
	if (nullptr != pArg)
		memmove(&m_tNaviDesc, pArg, sizeof(CNAVIGATION_DESC));
	SetUp_Neighbors();
	return S_OK;
}

void CNavigation::AddCell(const _float3* vPoints)
{
	CCell* pCell = CCell::Create(m_pDevice, m_pContext, vPoints, m_Cells.size());
	NULL_CHECK(pCell, E_FAIL);

	m_Cells.push_back(pCell);

}

_vector CNavigation::GetSlidingVector(_fvector vDir, _fvector vContactNormal)
{
	_vector N = XMVector3Normalize(vContactNormal); // 충돌직선의 법선 벡터
	_vector P = XMVector3Normalize(vDir); // 플레이어 이동벡터
	_vector S = P - N * XMVectorGetX(XMVector3Dot(P, N));
	return S; // 슬라이딩 벡터를 계산합니다.
}

_bool CNavigation::is_Move(_fvector vPosition)
{
	_int iNeighborIndex = { -1 };
	CCell::NEIGHBOR eNeighbor = { CCell::NEIGHBOR_END };
	// 내적해서 내 위치가 셀 안에 있는지 판단
	if (true == m_Cells[m_tNaviDesc.iCurrentIndex]->is_In(vPosition, &iNeighborIndex, eNeighbor))
	{
		return true;
	}
	else // 내가 인접한 이웃으로 셀을 벗어나려함.
	{
		// 이웃이 있다면
		if (-1 != iNeighborIndex)
		{
			while (true)
			{
				// 더 이상 이웃 인덱스가 존재하지 않으면 탈출
				if (-1 == iNeighborIndex)
					return false;

				// 이웃인덱스를 찾으면 탈출
				if (true == m_Cells[iNeighborIndex]->is_In(vPosition, &iNeighborIndex, eNeighbor))
					break;
			}	
			m_tNaviDesc.iCurrentIndex = iNeighborIndex;
			return true;
		}
	}

	// 이웃이 없다면
	// 슬라이딩 벡터를 구하기 위해 충돌한 직선의 법선벡터 반환
	m_vContactNormal = m_Cells[m_tNaviDesc.iCurrentIndex]->Get_Normal(eNeighbor);
	return false;
}

#ifdef _DEBUG
void CNavigation::Set_ShaderResources()
{
	NULL_CHECK(m_pShader);

	_float4x4 WorldMatrix;
	XMStoreFloat4x4(&WorldMatrix, XMMatrixIdentity());

	FAILED_CHECK(m_pShader->Bind_Matrix("g_WorldMatrix", &WorldMatrix));

	CPipeLine* pPipeLine = CPipeLine::GetInstance();
	Safe_AddRef(pPipeLine);

	_float4x4 ViewMatrix = pPipeLine->Get_TransformFloat4x4(CPipeLine::D3DTS_VIEW);
	FAILED_CHECK(m_pShader->Bind_Matrix("g_ViewMatrix", &ViewMatrix));

	_float4x4 ProjMatrix = pPipeLine->Get_TransformFloat4x4(CPipeLine::D3DTS_PROJ);
	FAILED_CHECK(m_pShader->Bind_Matrix("g_ProjMatrix", &ProjMatrix));

	_float4		vColor = -1 == m_tNaviDesc.iCurrentIndex ? _float4(0.f, 1.f, 0.f, 1.f) : _float4(1.f, 0.f, 0.f, 1.f);
	FAILED_CHECK(m_pShader->Bind_RawValue("g_vColor", &vColor, sizeof(_float4)));

	Safe_Release(pPipeLine);
	return;
}

void CNavigation::UpdateCellsCollider()
{
	for (auto& Cell : m_Cells)
		Cell->UpdateColliderForVertices();
}
void CNavigation::UpdateCellCollider(_uint iIndex)
{
	if (iIndex > m_Cells.size())
		return;

	m_Cells[iIndex]->UpdateColliderForVertices();
}
#endif

#ifdef _DEBUG
HRESULT CNavigation::Render_Navigation()
{
	NULL_CHECK_RETURN(m_pShader, E_FAIL);

	Set_ShaderResources();

	m_pShader->Begin(0);

	for (auto& pCell : m_Cells)
	{
		pCell->Render_VIBuffer();
	}

	for (auto& pCell : m_Cells)
	{
		pCell->Render_ColliderSphere();
	}

	return S_OK;
}
_bool CNavigation::IsCellVertexPicked(vector<CELL_PICK_DESC>& tCellPickDesces, const RAY& tRay)
{
	_bool bResult = { false };
	for (auto& Cell : m_Cells)
	{
		CELL_PICK_DESC tCellPickDesc;
		if (bResult = Cell->IsCellVertexPicked(tCellPickDesc, tRay))
		{
			tCellPickDesces.push_back(tCellPickDesc);
		}
	}
	return bResult;
}

_bool CNavigation::IsCellPicked(CELL_PICK_DESC& tPickDesc, const RAY& tRay)
{
	_bool bResult = { false };
	for (auto& CellPickDesc : m_Cells)
	{
		bResult = CellPickDesc->IsCellPicked(tPickDesc, tRay);
	}
	return bResult;
}
#endif // _DEBUG

void CNavigation::SetUp_Neighbors()
{
	for (auto& pSourCell : m_Cells)
	{
		for (auto& pDestCell : m_Cells)
		{
			if (pSourCell == pDestCell)
				continue;

			if (true == pDestCell->Compare_Points(pSourCell->Get_Point(CCell::POINT_A), pSourCell->Get_Point(CCell::POINT_B)))
			{
				pSourCell->SetUp_Neighbor(CCell::NEIGHBOR_AB, pDestCell);
			}

			if (true == pDestCell->Compare_Points(pSourCell->Get_Point(CCell::POINT_B), pSourCell->Get_Point(CCell::POINT_C)))
			{
				pSourCell->SetUp_Neighbor(CCell::NEIGHBOR_BC, pDestCell);
			}

			if (true == pDestCell->Compare_Points(pSourCell->Get_Point(CCell::POINT_C), pSourCell->Get_Point(CCell::POINT_A)))
			{
				pSourCell->SetUp_Neighbor(CCell::NEIGHBOR_CA, pDestCell);
			}
		}
	}
}

void CNavigation::Release_Cells()
{
	for (auto& pCell : m_Cells)
		Safe_Release(pCell);
	m_Cells.clear();
}

CNavigation* CNavigation::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _tchar* pNavigationDataFiles)
{
	CNavigation* pInstance = new CNavigation(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(pNavigationDataFiles)))
	{
		MSG_BOX("Failed to Created CNavigation");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CNavigation::Clone(void* pArg)
{
	CNavigation* pInstance = new CNavigation(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned CNavigation");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CNavigation::Free()
{
	__super::Free();

	Release_Cells();

#ifdef _DEBUG
	Safe_Release(m_pShader);
#endif // _DEBUG
}



