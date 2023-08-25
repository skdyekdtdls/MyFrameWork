#include "Layer.h"
#include "GameObject.h"
#include "GameInstance.h"

CLayer::CLayer()
	: m_tInfo()
{
	
}

void CLayer::Save(HANDLE hFile, DWORD& dwByte)
{
	m_tInfo.Save(hFile, dwByte);
	for (auto& GameObejct : m_pGameObjects)
	{
		ISerializable* pSerializableGameObject = dynamic_cast<ISerializable*>(GameObejct);
		if (nullptr == pSerializableGameObject)
			continue;
		WriteEnable(true);
		
		pSerializableGameObject->Save(hFile, dwByte);
	}
	WriteEnable(false);
}

void CLayer::Load(HANDLE hFile, DWORD& dwByte, _uint iLevelIndex)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);
	
	while (true)
	{
		if (false == ReadEnable())
			break;

		INFO tTagInfo;
		tTagInfo.Load(hFile, dwByte);
		CGameObject* pGameObject = { nullptr };
		pGameObject = pGameInstance->Add_GameObject(iLevelIndex, tTagInfo.wstrKey.c_str(), m_tInfo.wstrName.c_str());
		NULL_CHECK(pGameObject);
		ISerializable* pSerializableGameObj = { nullptr };
		pSerializableGameObj = dynamic_cast<ISerializable*>(pGameObject);
		NULL_CHECK(pSerializableGameObj);
		pSerializableGameObj->Load(hFile, dwByte, iLevelIndex);
	}

	Safe_Release(pGameInstance);
}

HRESULT CLayer::Add_GameObject(CGameObject* pGameObject)
{
	NULL_CHECK_RETURN(pGameObject, E_FAIL);

	m_pGameObjects.push_back(pGameObject);
	pGameObject->SetLayerName(m_tInfo.wstrName);
	return S_OK;
}

HRESULT CLayer::Delete_GameObject(string strName)
{
	for (auto iter = m_pGameObjects.begin(); iter != m_pGameObjects.end();)
	{
		if (strName == (*iter)->Get_Name())
		{
			Safe_Release(*iter);
			iter = m_pGameObjects.erase(iter);
			return S_OK;
		}
		else
			++iter;
	}
	return E_FAIL;
}

void CLayer::Tick(_double TimeDelta)
{
	ReleaseGameObject();
	
	for (auto& GameObject : m_pGameObjects)
	{
		if (m_tInfo.wstrName == L"Layer_Bullet")
		{
			int a = 0;
		}
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
			string tmp = pObj->Get_Name();
			tmp.pop_back();
			return strName == tmp;
		});

	return (GameObject != m_pGameObjects.end()) ? *GameObject : nullptr;
}

void CLayer::ReleaseGameObject()
{
	for (auto iter = m_pGameObjects.begin(); iter != m_pGameObjects.end();)
	{
		if (true == (*iter)->GetDead())
		{
			Safe_Release(*iter);
			iter = m_pGameObjects.erase(iter);
		}
		else
			++iter;
	}
}

CLayer* CLayer::Create(wstring wstrLayerName)
{
	CLayer* pLayer = new CLayer;
	pLayer->SetLayerName(wstrLayerName);
	return pLayer;
}

void CLayer::Free()
{
	for (auto& pGameObject : m_pGameObjects)
		Safe_Release(pGameObject);
	
	m_pGameObjects.clear();
}
