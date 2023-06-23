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
			string tmp = pObj->Get_Name();
			tmp.pop_back();
			return strName == tmp;
		});

	return (GameObject != m_pGameObjects.end()) ? *GameObject : nullptr;
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
