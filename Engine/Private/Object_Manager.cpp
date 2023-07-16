#include "Object_Manager.h"
#include "GameObject.h"
#include "Layer.h"
#include "Component.h"
#include "Level_Manager.h"
#include "Camera.h"
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

CGameObject* CObject_Manager::Add_GameObject(_uint iLevelIndex, const _tchar* pPrototypeTag, const _tchar* pLayerTag, void* pArg)
{
	CGameObject* pPrototype = Find_Prototype(pPrototypeTag);
	if (nullptr == pPrototype)
		return nullptr;

	CGameObject* pGameObject = pPrototype->Clone(pArg);

	if (nullptr == pGameObject)
	{
		Safe_Release(pGameObject);
		return nullptr;
	}

	CLayer* pLayer = Find_LayerByName(iLevelIndex, pLayerTag);
	if (nullptr == pLayer)
	{
		pLayer = CLayer::Create(pLayerTag);
		m_pLayers[iLevelIndex].emplace(pLayerTag, pLayer);
	}

	pLayer->Add_GameObject(pGameObject);

	return pGameObject;
}

HRESULT CObject_Manager::Delete_GameObject(_uint iLevelIndex, const _tchar* pLayerTag, string strName)
{
	CLayer* pLayer = Find_LayerByName(iLevelIndex, pLayerTag);

	if (nullptr == pLayer)
		return E_FAIL;

	return pLayer->Delete_GameObject(strName);
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

const list<CGameObject*>& CObject_Manager::GetObjListOfLayer(const _tchar* pTag, _uint iLevelIndex)
{
	CLayer* pLayer = Find_LayerByName(iLevelIndex, pTag);
	return pLayer->GetGameObjects();
}

void CObject_Manager::Tick(_double TimeDelta)
{
	for (_uint i = 0; i < m_iNumLevels; ++i)
	{
		for (auto& Pair : m_pLayers[i])
			Pair.second->Tick(TimeDelta);
	}
}

void CObject_Manager::Late_Tick(_double TimeDelta)
{
	for (_uint i = 0; i < m_iNumLevels; ++i)
	{
		for (auto& Pair : m_pLayers[i])
			Pair.second->Late_Tick(TimeDelta);
	}
}

CGameObject* CObject_Manager::Find_Prototype(const _tchar* pPrototypeTag)
{
	auto pPrototypeKey = find_if(m_Prototypes.begin(), m_Prototypes.end(), CTag_Finder(pPrototypeTag));

	if (m_Prototypes.end() == pPrototypeKey)
		return nullptr;

	return pPrototypeKey->second;
}

CLayer* CObject_Manager::Find_LayerByName(_uint iLevelIndex, const _tchar* pLayerTag)
{
	auto pLayerKey = find_if(m_pLayers[iLevelIndex].begin(), m_pLayers[iLevelIndex].end(), CTag_Finder(pLayerTag));
	
	if (m_pLayers[iLevelIndex].end() == pLayerKey)
		return nullptr;

	return pLayerKey->second;
}

CGameObject* CObject_Manager::Clone_GameObject(const _tchar* pPrototypeTag, void* pArg)
{
	CGameObject* pPrototype = Find_Prototype(pPrototypeTag);
	if (nullptr == pPrototype)
		return nullptr;

	CGameObject* pGameObject = pPrototype->Clone(pArg);
	if (nullptr == pGameObject)
		return nullptr;

	return pGameObject;
}

void CObject_Manager::Free()
{
	for (_uint i = 0; i < m_iNumLevels; ++i)
	{
		for (auto& Pair : m_pLayers[i])
		{
			Safe_Release(Pair.second);
		}
		m_pLayers[i].clear();
	}
	Safe_Delete_Array(m_pLayers);
	
	for (auto& Pair : m_Prototypes)
	{
		Safe_Release(Pair.second);
	}
	m_Prototypes.clear();
}

void CObject_Manager::Serialization(HANDLE hFile, DWORD& dwByte, _uint iLevelIndex)
{
	_uint iSize = m_pLayers[iLevelIndex].size();
	WriteVoid(&iSize, sizeof(_uint));

	for (auto& Layer : m_pLayers[iLevelIndex])
	{
		Layer.second->Save(hFile, dwByte);
	}
}

void CObject_Manager::Deserialization(HANDLE hFile, DWORD& dwByte, _uint iLevelIndex)
{
	for (auto& Pair : m_pLayers[iLevelIndex])
	{
		Safe_Release(Pair.second);
	}
	m_pLayers[iLevelIndex].clear();

	_uint iSize = 0;
	ReadVoid(&iSize, sizeof(_uint));

	for(_uint i = 0 ; i < iSize; ++i)
	{			
		INFO tTagInfo;
		tTagInfo.Load(hFile, dwByte);
		CLayer* pLayer = CLayer::Create(tTagInfo.wstrName);
		m_pLayers[iLevelIndex].emplace(pLayer->GetLayerName(), pLayer);
		pLayer->Load(hFile, dwByte, iLevelIndex);
	}	
}

CComponent* CObject_Manager::Get_ComponentOfClone(_uint iLevelIndex, const _tchar* pLayerTag, string pCloneObjName, const _tchar* pCloneComName)
{
	CLayer* pLayer = Find_LayerByName(iLevelIndex, pLayerTag);
	NULL_CHECK_RETURN(pLayer, nullptr);
	CGameObject* pGameObject = pLayer->FindByName(pCloneObjName);
	NULL_CHECK_RETURN(pGameObject, nullptr)
	CComponent* pComponent = pGameObject->Get_Component(pCloneComName);
	NULL_CHECK_RETURN(pComponent, nullptr)
	return pComponent;
}