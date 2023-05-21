#include "Level_Manager.h"
#include "Level.h"
#include "GameInstance.h"
IMPLEMENT_SINGLETON(CLevel_Manager)

CLevel_Manager::CLevel_Manager()
{

}

HRESULT CLevel_Manager::Open_Level(_uint iLevelindex, CLevel* pNewLevel)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);
	
	if(nullptr != m_pCurrentLevel)
		pGameInstance->Clear_LevelResources(m_iLevelindex);
	
	Safe_Release(pGameInstance);

	Safe_Release(m_pCurrentLevel);

	m_pCurrentLevel = pNewLevel;

	m_iLevelindex = iLevelindex;

	return S_OK;
}

void CLevel_Manager::Tick(_double TimeDelta)
{
	if (nullptr == m_pCurrentLevel)
		return;

	m_pCurrentLevel->Tick(TimeDelta);
}

void CLevel_Manager::Late_Tick(_double TimeDelta)
{
	if (nullptr == m_pCurrentLevel)
		return;

	m_pCurrentLevel->Late_Tick(TimeDelta);
}

void CLevel_Manager::Free()
{
	Safe_Release(m_pCurrentLevel);
}
