#ifdef _DEBUG
#include "ImWindow_SaveLoads.h"
#include "GameInstance.h"
CImWindow_SaveLoads::CImWindow_SaveLoads(ImGuiIO* pIO)
	: CImWindow(pIO)
{

}

HRESULT CImWindow_SaveLoads::Initialize(void* pArg)
{
	if (__super::Initialize(pArg))
		return E_FAIL;

	return S_OK;
}

void CImWindow_SaveLoads::Tick()
{
	ImGui::Begin("MapTool");


	if (ImGui::Button("Save Level_GamePlay"))
	{
		Serialization(TEXT("../Bin/Data/Level/Level_GamePlay.dat"), LEVEL_IMGUI);
	}
	ImGui::SameLine();
	if (ImGui::Button("Load Level_GamePlay"))
	{
		Deserialization(TEXT("../Bin/Data/Level/Level_GamePlay.dat"), LEVEL_IMGUI);
	}

	ImGui::End();
}

void CImWindow_SaveLoads::Serialization(const _tchar* pSaveFileName, _uint iLevelIndex)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);
	HANDLE hFile = CreateFile(pSaveFileName, GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
	if (0 == hFile)
		assert(false);

	DWORD dwByte = { 0 };

	pGameInstance->Serialization(hFile, dwByte, iLevelIndex);

	CloseHandle(hFile);
	Safe_Release(pGameInstance);
}

void CImWindow_SaveLoads::Deserialization(const _tchar* pSaveFileName, _uint iLevelIndex)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);
	HANDLE hFile = CreateFile(pSaveFileName, GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	
	CRenderer* pRenderer = static_cast<CRenderer*>(pGameInstance->Get_ProtoComponent(LEVEL_STATIC, CRenderer::ProtoTag()));
	pRenderer->Draw_RenderGroup();
	if (0 == hFile)
		assert(false);

	DWORD dwByte = { 0 };
	pGameInstance->Deserialization(hFile, dwByte, iLevelIndex);

	CloseHandle(hFile);
	Safe_Release(pGameInstance);
}

CImWindow_SaveLoads* CImWindow_SaveLoads::Create(ImGuiIO* pIO)
{
	CImWindow_SaveLoads* pInstance = new CImWindow_SaveLoads(pIO);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created CImWindow_SaveLoads");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CImWindow_SaveLoads::Free()
{
	__super::Free();
}
#endif