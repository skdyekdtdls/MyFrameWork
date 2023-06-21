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

#ifdef _DEBUG
	m_pShader = CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_Navigation.hlsl"), VTXPOS_DECL::Elements, VTXPOS_DECL::iNumElements);
	NULL_CHECK_RETURN(m_pShader, E_FAIL);
#endif // _DEBUG

	return S_OK;
}

HRESULT CNavigation::Initialize(void* pArg)
{
	return S_OK;
}

void CNavigation::AddCell(const _float3* vPoints)
{
	CCell* pCell = CCell::Create(m_pDevice, m_pContext, vPoints, m_Cells.size());
	NULL_CHECK(pCell, E_FAIL);

	m_Cells.push_back(pCell);
}

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

#ifdef _DEBUG
HRESULT CNavigation::Render_Navigation()
{
	NULL_CHECK_RETURN(m_pShader, E_FAIL);
	Set_ShaderResources();

	m_pShader->Begin(0);

	for (auto& pCell : m_Cells)
		pCell->Render();

	return S_OK;
}
#endif // _DEBUG

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

	for (auto& pCell : m_Cells)
		Safe_Release(pCell);
	m_Cells.clear();
#ifdef _DEBUG
	Safe_Release(m_pShader);
#endif // _DEBUG
}

void CNavigation::Save(HANDLE hFile, DWORD& dwByte)
{
	_uint iSize = m_Cells.size();
	WriteVoid(&iSize, sizeof(_uint));
	for (_uint i = 0; i < iSize; ++i)
		m_Cells[i]->Save(hFile, dwByte);
}

void CNavigation::Load(HANDLE hFile, DWORD& dwByte)
{
	for (auto& pCell : m_Cells)
		Safe_Release(pCell);
	m_Cells.clear();

	_uint iSize = { 0 };
	ReadVoid(&iSize, sizeof(_uint));
	for (_uint i = 0; i < iSize; ++i)
	{
		_float3 vPoint[CCell::POINT_END];
		ZeroStruct(vPoint);
		AddCell(vPoint); // Push_back After Creating
		m_Cells[i]->Load(hFile, dwByte);
	}
}

