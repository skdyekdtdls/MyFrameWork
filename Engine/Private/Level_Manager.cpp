#include "Level_Manager.h"
#include "Level.h"

IMPLEMENT_SINGLETON(CLevel_Manager)

CLevel_Manager::CLevel_Manager()
{

}

HRESULT CLevel_Manager::Open_Level(CLevel* pNewLevel)
{
	return S_OK;
}

void CLevel_Manager::Free()
{
}
