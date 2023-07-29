#include "MainApp.h"
#include "GameInstance.h"
#include "Level.h"
#include "Level_Loading.h"
#include "ClientInstance.h"
#include "CrystalGolemAttackAreaBullet.h"
#include "CrystalGolemAttackArea02Bullet.h"
#include "CrystalGolemAttackRangeBullet.h"

#ifdef _DEBUG
#include "ImWindow_Manager.h"
#endif // DEBUG

CMainApp::CMainApp()
	: m_pGameInstance(CGameInstance::GetInstance())
{
	Safe_AddRef(m_pGameInstance);
}

HRESULT CMainApp::Initialize()
{
	GRAPHICDESC GraphicDesc;
	ZeroStruct(GraphicDesc);

	GraphicDesc.hWnd = g_hWnd;
	GraphicDesc.hInst = g_hInst;
	GraphicDesc.iViewportSizeX = 1264;
	GraphicDesc.iViewportSizeY = 681;
	GraphicDesc.eWinMode = GraphicDesc.WM_WIN;

	if (FAILED(m_pGameInstance->Initialize_Engine(LEVEL_END, GraphicDesc, &m_pDevice, &m_pContext)))
		return E_FAIL;

#ifdef _DEBUG
	CImWindow_Manager::GetInstance()->Initialize(&m_pIO, m_pDevice, m_pContext);
#endif

	if (FAILED(Ready_Prototype_Component_For_Static()))
		return E_FAIL;
#ifdef _DEBUG
	if (FAILED(Open_Level(LEVEL_IMGUI)))
		return E_FAIL;
#else
	if(FAILED(Open_Level(LEVEL_LOGO)))
		return E_FAIL;
#endif
	//if (FAILED(Ready_Gara()))
	//	return E_FAIL;

	if (FAILED(Ready_Font()))
		return E_FAIL;

	Ready_Pool();

	return S_OK;
}

void CMainApp::Tick(_double TimeDelta)
{
	if (nullptr == m_pGameInstance)
		return;

	m_pGameInstance->Tick_Engine(TimeDelta);
}

HRESULT CMainApp::Render()
{
	if (nullptr == m_pGameInstance ||
		nullptr == m_pRenderer)
	{
		assert(false);
		return E_FAIL;
	}

	m_pGameInstance->Clear_BackBuffer_View(_float4(0.f, 0.f, 1.f, 1.f));
	m_pGameInstance->Clear_DepthStencil_View();

	m_pRenderer->Draw_RenderGroup();

#ifdef _DEBUG
	CImWindow_Manager::GetInstance()->Render();
#endif
	// TODO
	m_pGameInstance->Present();

	return S_OK;
}

HRESULT CMainApp::Ready_Prototype_Component_For_Static()
{
	if (nullptr == m_pGameInstance)
		return E_FAIL;

	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"),
		m_pRenderer = CRenderer::Create(m_pDevice, m_pContext)), E_FAIL);

	// ���̴� �غ�
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxAnimMesh"),
		CShader::Create(m_pDevice, m_pContext, L"../Bin/ShaderFiles/Shader_VtxAnimMesh.hlsl"
			, VTXANIMMESH_DECL::Elements, VTXANIMMESH_DECL::iNumElements)), E_FAIL);

	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxCol"),
		CShader::Create(m_pDevice, m_pContext, L"../Bin/ShaderFiles/Shader_VtxCol.hlsl"
			, VTXPOSCOL_DECL::Elements, VTXPOSCOL_DECL::iNumElements)), E_FAIL);

	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxCube"),
		CShader::Create(m_pDevice, m_pContext, L"../Bin/ShaderFiles/Shader_VtxCube.hlsl"
			, VTXCUBE_DECL::Elements, VTXCUBE_DECL::iNumElements)), E_FAIL);

	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxMesh"),
		CShader::Create(m_pDevice, m_pContext, L"../Bin/ShaderFiles/Shader_VtxMesh.hlsl"
			, VTXMESH_DECL::Elements, VTXMESH_DECL::iNumElements)), E_FAIL);

	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxNorTex"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxNorTex.hlsl")
			, VTXPOSNORTEX_DECL::Elements, VTXPOSNORTEX_DECL::iNumElements)), E_FAIL);

	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxPointInstance"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxPointInstance.hlsl")
			, VTXPOINTINSTANCE_DECL::Elements, VTXPOINTINSTANCE_DECL::iNumElements)), E_FAIL);

	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_Vtxtex"),
		CShader::Create(m_pDevice, m_pContext, L"../Bin/ShaderFiles/Shader_Vtxtex.hlsl"
			, VTXPOSTEX_DECL::Elements, VTXPOSTEX_DECL::iNumElements)), E_FAIL);

	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxTexInstance"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxTexInstance.hlsl")
			, VTXRECTINSTANCE_DECL::Elements, VTXRECTINSTANCE_DECL::iNumElements)), E_FAIL);

	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(LEVEL_STATIC, CVIBuffer_Rect::ProtoTag(),
		CVIBuffer_Rect::Create(m_pDevice, m_pContext)), E_FAIL);

	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(LEVEL_STATIC, CVIBuffer_Cube::ProtoTag(),
		CVIBuffer_Cube::Create(m_pDevice, m_pContext)), E_FAIL);

	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Transform"),
		CTransform::Create(m_pDevice, m_pContext)), E_FAIL);

	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(LEVEL_STATIC, CColliderAABB::ProtoTag(),
		CColliderAABB::Create(m_pDevice, m_pContext)), E_FAIL);

	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(LEVEL_STATIC, CColliderSphere::ProtoTag(),
		CColliderSphere::Create(m_pDevice, m_pContext)), E_FAIL);

	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(LEVEL_STATIC, Raycast::ProtoTag(),
		Raycast::Create(m_pDevice, m_pContext)), E_FAIL);

	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TimeCounter::ProtoTag(),
		TimeCounter::Create(m_pDevice, m_pContext)), E_FAIL);

	Safe_AddRef(m_pRenderer);

	return S_OK;
}

HRESULT CMainApp::Ready_Font()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	if (FAILED(m_pGameInstance->Add_Fonts(m_pDevice, m_pContext, TEXT("Font_135"), TEXT("../../Resources/Fonts/135ex.spritefont"))))
	{
		Safe_Release(pGameInstance);
		return E_FAIL;
	}	

	Safe_Release(pGameInstance);
	return S_OK;
}

HRESULT CMainApp::Ready_Gara()
{
	if (nullptr == m_pDevice)
		return E_FAIL;

	_float3		vPoints[3];

	_ulong		dwByte = { 0 };
	HANDLE		hFile = { 0 };

	hFile = CreateFile(TEXT("../Bin/Data/Navigation.dat"), GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
	if (0 == hFile)
		return E_FAIL;

	vPoints[2] = _float3(0.0f, 0.f, 5.f);
	vPoints[1] = _float3(5.f, 0.f, 0.f);
	vPoints[0] = _float3(0.f, 0.f, 0.f);
	WriteFile(hFile, vPoints, sizeof(_float3) * 3, &dwByte, nullptr);

	vPoints[2] = _float3(0.0f, 0.f, 5.f);
	vPoints[1] = _float3(5.f, 0.f, 5.f);
	vPoints[0] = _float3(5.f, 0.f, 0.f);
	WriteFile(hFile, vPoints, sizeof(_float3) * 3, &dwByte, nullptr);

	vPoints[2] = _float3(0.0f, 0.f, 10.0f);
	vPoints[1] = _float3(5.f, 0.f, 5.f);
	vPoints[0] = _float3(0.f, 0.f, 5.f);
	WriteFile(hFile, vPoints, sizeof(_float3) * 3, &dwByte, nullptr);

	vPoints[2] = _float3(5.f, 0.f, 5.f);
	vPoints[1] = _float3(10.f, 0.f, 0.f);
	vPoints[0] = _float3(5.f, 0.f, 0.f);
	WriteFile(hFile, vPoints, sizeof(_float3) * 3, &dwByte, nullptr);

	CloseHandle(hFile);

	return S_OK;
}

HRESULT CMainApp::Ready_Pool()
{
	ObjectPool<class ClintBasicBullet>::GetInstance()->Initialize(m_pDevice, m_pContext);
	ObjectPool<class BatPotato_RIGBullet>::GetInstance()->Initialize(m_pDevice, m_pContext);
	ObjectPool<class CannonSpiderBullet>::GetInstance()->Initialize(m_pDevice, m_pContext);
	ObjectPool<class CrystalGolemAttackAreaBullet>::GetInstance()->Initialize(m_pDevice, m_pContext);
	ObjectPool<class CrystalGolemAttackArea02Bullet>::GetInstance()->Initialize(m_pDevice, m_pContext);
	ObjectPool<class CrystalGolemAttackRangeBullet>::GetInstance()->Initialize(m_pDevice, m_pContext);

	return S_OK; // Ǯ �̴ϼ� �� ����.
}

HRESULT CMainApp::Open_Level(LEVELID eLevelIndex)
{
	if (nullptr == m_pGameInstance)
		return E_FAIL;

	return m_pGameInstance->Open_Level(LEVEL_LOADING, CLevel_Loading::Create(m_pDevice, m_pContext, eLevelIndex));
}

CMainApp* CMainApp::Create()
{
	CMainApp* pInstance = new CMainApp();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created CMainApp");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CMainApp::Free()
{
	Safe_Release(m_pRenderer);
	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
	Safe_Release(m_pGameInstance);
	ObjectPool<class ClintBasicBullet>::DestroyInstance();
	ObjectPool<class BatPotato_RIGBullet>::DestroyInstance();
	ObjectPool<class CannonSpiderBullet>::DestroyInstance();
	ObjectPool<class CrystalGolemAttackAreaBullet>::DestroyInstance();
	ObjectPool<class CrystalGolemAttackArea02Bullet>::DestroyInstance();
	ObjectPool<class CrystalGolemAttackRangeBullet>::DestroyInstance();
#ifdef _DEBUG
	CImWindow_Manager::DestroyInstance();
#endif // DEBUG
	ClientInstance::DestroyInstance();
	CGameInstance::Release_Engine();
}
