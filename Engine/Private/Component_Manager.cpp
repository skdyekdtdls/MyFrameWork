#include "Component_Manager.h"

IMPLEMENT_SINGLETON(CComponent_Manager)

CComponent_Manager::CComponent_Manager()
{

}

HRESULT CComponent_Manager::Reserve_Container(_uint iLevelIndex)
{
	if (nullptr != m_pComponents)
		return E_FAIL;

	m_iNumLevels = iLevelIndex;

	m_pComponents = new COMPONENTS[m_iNumLevels];

	if (nullptr == m_pComponents)
		return E_FAIL;

	return S_OK;
}

HRESULT CComponent_Manager::Add_Prototype(_uint iLevelIndex, const _tchar* pPrototypeTag, CComponent* pCompoent)
{
	if (nullptr != pCompoent)
		return E_FAIL;

	CComponent* pPrototype = Find_Component(iLevelIndex, pPrototypeTag);

	if (nullptr != pPrototype)
		return E_FAIL;

	m_pComponents->emplace(pPrototypeTag, pCompoent);

	return S_OK;
}

CComponent* CComponent_Manager::Clone_Component(_uint iLevelIndex, const _tchar* pPrototypeTag, void* pArg)
{
	CComponent* pPrototype = Find_Component(iLevelIndex, pPrototypeTag);

	if (nullptr == pPrototype)
		return nullptr;

	CComponent* pComponent = pPrototype->Clone(pArg);

	if (nullptr == pComponent)
	{
		Safe_Release(pComponent);
		return nullptr;
	}
		
	return pComponent;
}

void CComponent_Manager::Clear_LevelResources(_uint iLevelIndex)
{
	if (iLevelIndex >= m_iNumLevels)
		return;

	for (auto& Pair : m_pComponents[iLevelIndex])
	{
		Safe_Release(Pair.second);
	}
	m_pComponents->clear();
}

CComponent* CComponent_Manager::Find_Component(_uint iLevelIndex, const _tchar* pPrototypeTag)
{
	auto iter = find_if(m_pComponents[iLevelIndex].begin(), m_pComponents[iLevelIndex].end(), CTag_Finder(pPrototypeTag));
	
	if (iter == m_pComponents[iLevelIndex].end())
		return nullptr;

	return iter->second;
}

void CComponent_Manager::Free(void)
{
	for (_uint i = 0; i < m_iNumLevels; ++i)
	{
		Clear_LevelResources(i);
	}
	Safe_Delete_Array(m_pComponents);
}
