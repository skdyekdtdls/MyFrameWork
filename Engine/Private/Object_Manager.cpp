#include "Object_Manager.h"
#include "GameObject.h"
#include "Layer.h"
IMPLEMENT_SINGLETON(CObject_Manager)

CObject_Manager::CObject_Manager()
{

}

HRESULT CObject_Manager::Reserve_Containers(_uint iNumLevels)
{
	if (nullptr != m_pLayers)
		return E_FAIL;

	m_iNumLevels = iNumLevels;

	m_pLayers = new LAYERS[m_iNumLevels];

	return S_OK;
}

HRESULT CObject_Manager::Add_Prototype(const _tchar* pPrototypeTag, CGameObject* pGameObject)
{
	if (nullptr != Find_Prototype(pPrototypeTag))
		return E_FAIL;

	m_Prototypes.emplace(pPrototypeTag, pGameObject);

	return S_OK;
}

HRESULT CObject_Manager::Add_GameObject(_uint iLevelIndex, const _tchar* pPrototypeTag, const _tchar* pLayerTag, void* pArg)
{
	CGameObject* pPrototype = Find_Prototype(pPrototypeTag);
	if (nullptr == pPrototype)
		return E_FAIL;

	CGameObject* pGameObject = pPrototype->Clone(pArg);

	if (nullptr == pGameObject)
	{
		Safe_Release(pGameObject);
		return E_FAIL;
	}

	CLayer* pLayer = Find_Layer(iLevelIndex, pLayerTag);
	if (nullptr == pLayer)
	{
		pLayer = CLayer::Create();
		m_pLayers[iLevelIndex].emplace(pLayerTag, pLayer);
	}

	pLayer->Add_GameObject(pGameObject);

	return S_OK;
}

void CObject_Manager::Clear_LevelResources(_uint iLevelIndex)
{
	if (iLevelIndex >= m_iNumLevels)
		return;

	for (auto& Pair : m_pLayers[iLevelIndex])
	{
		Safe_Release(Pair.second);
	}
	m_pLayers[iLevelIndex].clear();
}

CGameObject* CObject_Manager::Find_Prototype(const _tchar* pPrototypeTag)
{
	auto pPrototypeKey = find_if(m_Prototypes.begin(), m_Prototypes.end(), CTag_Finder(pPrototypeTag));

	if (m_Prototypes.end() == pPrototypeKey)
		return nullptr;

	return pPrototypeKey->second;
}

CLayer* CObject_Manager::Find_Layer(_uint iLevelIndex, const _tchar* pLayerTag)
{
	auto pLayerKey = find_if(m_pLayers[iLevelIndex].begin(), m_pLayers[iLevelIndex].end(), CTag_Finder(pLayerTag));
	
	if (m_pLayers[iLevelIndex].end() == pLayerKey)
		return nullptr;

	return pLayerKey->second;
}

void CObject_Manager::Free()
{
	for (_uint i = 0; i < m_iNumLevels; ++i)
	{
		Clear_LevelResources(i);
	}
	Safe_Delete_Array(m_pLayers);
	

	for (auto& Pair : m_Prototypes)
	{
		Safe_Release(Pair.second);
	}
	m_Prototypes.clear();
}
