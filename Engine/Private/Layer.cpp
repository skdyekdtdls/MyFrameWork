#include "Layer.h"
#include "GameObject.h"

CLayer::CLayer()
{
}

HRESULT CLayer::Add_GameObject(CGameObject* pGameObject)
{
	if (nullptr == pGameObject)
		return E_FAIL;

	m_pGameObjects.push_back(pGameObject);

	return S_OK;
}

void CLayer::Tick(_double TimeDelta)
{
	for (auto& GameObject : m_pGameObjects)
	{
		GameObject->Tick(TimeDelta);
	}
}

void CLayer::Late_Tick(_double TimeDelta)
{
	for (auto& GameObject : m_pGameObjects)
	{
		GameObject->Late_Tick(TimeDelta);
	}
}

CGameObject* CLayer::FindByName(string strName)
{
	auto GameObject = find_if(m_pGameObjects.begin(), m_pGameObjects.end(), [&strName](CGameObject* pObj)
		{
			return strName == pObj->Get_Name();
		});

	return (GameObject != m_pGameObjects.end()) ? *GameObject : nullptr;
}


CLayer* CLayer::Create()
{
	return new CLayer;
}

void CLayer::Free()
{
	for (auto& pGameObject : m_pGameObjects)
		Safe_Release(pGameObject);

	m_pGameObjects.clear();
}
