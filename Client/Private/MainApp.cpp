#include "MainApp.h"

CMainApp::CMainApp()
{
}

HRESULT CMainApp::Initialize()
{
	return E_FAIL;
}

void CMainApp::Tick(_double TimeDelta)
{
}

HRESULT CMainApp::Render()
{
	return S_OK;
}

CMainApp* CMainApp::Create()
{
	CMainApp* pInstance = new CMainApp();

	if (FAILED(pInstance->Initialize()))
	{
		CONSOLE_MSG("�ܼ� �޽��� �׽�Ʈ");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CMainApp::Free()
{
}
