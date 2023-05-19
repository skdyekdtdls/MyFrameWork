#include "MainApp.h"
#include "GameInstance.h"

CMainApp::CMainApp()
	: m_pGameInstance(CGameInstance::GetInstance())
{
}

HRESULT CMainApp::Initialize()
{
	GRAPHICDESC GraphicDesc;
	GraphicDesc.hWnd = g_hWnd;
	GraphicDesc.eWinMode = GraphicDesc.WM_WIN;
	GraphicDesc.iViewportSizeX = g_iWinSizeX;
	GraphicDesc.iViewportSizeY = g_iWinSizeY;

	if (FAILED(m_pGameInstance->Initialize_Engine(GraphicDesc, &m_pDevice, &m_pDeviceContext)))
		return E_FAIL;

	return S_OK;
}

void CMainApp::Tick(_double TimeDelta)
{
}

HRESULT CMainApp::Render()
{
	if (nullptr == m_pGameInstance)
		return E_FAIL;

	m_pGameInstance->Clear_BackBuffer_View(_float4(0.f, 0.f, 1.f, 1.f));
	m_pGameInstance->Clear_DepthStencil_View();

	// TODO

	m_pGameInstance->Present();

	return S_OK;
}



CMainApp* CMainApp::Create()
{
	CMainApp* pInstance = new CMainApp();

	if (FAILED(pInstance->Initialize()))
	{
		CONSOLE_MSG("콘솔 메시지 테스트");
		Safe_Release(pInstance);
	}
	return pInstance;
}
//
void CMainApp::Free()
{
	Safe_Release(m_pDevice);
	Safe_Release(m_pDeviceContext);
	CGameInstance::Release_Engine();
}
