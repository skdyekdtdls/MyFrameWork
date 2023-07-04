#pragma once

#include "Base.h"
#include "ISerializable.h"

BEGIN(Engine)
class CGameObject;
class CLayer;
class CComponent;
class CObject_Manager final : public CBase
{
	DECLARE_SINGLETON(CObject_Manager)
public:
	typedef unordered_map<const _tchar*, CLayer*> LAYERS;
public:
	CObject_Manager();
	~CObject_Manager() = default;
public:
	void Serialization(HANDLE hFile, DWORD& dwByte, _uint iLevelIndex = 1);
	void Deserialization(HANDLE hFile, DWORD& dwByte, _uint iLevelIndex);
	
	CComponent* Get_ComponentOfClone(_uint iLevelIndex, const _tchar* pLayerTag, string pCloneObjName, const _tchar* pCloneComName);

	const list<CGameObject*>& GetObjListOfLayer(const _tchar* pTag, _uint iLevelIndex = 1);
	_uint GetNumLayers(_uint iLevelIndex) {
		return m_pLayers[iLevelIndex].size();
	}

	LAYERS* GetLayers() {
		return m_pLayers;
	}

public:
	HRESULT Reserve_Containers(_uint iNumLevels);
	HRESULT Add_Prototype(const _tchar* pPrototypeTag, CGameObject* pGameObject);
	CGameObject* Add_GameObject(_uint iLevelIndex, const _tchar* pPrototypeTag, const _tchar* pLayerTag, void* pArg);
	void Clear_LevelResources(_uint iLevelIndex);

	void Tick(_double TimeDelta);
	void Late_Tick(_double TimeDelta);
	CLayer* Find_LayerByName(_uint iLevelIndex, const _tchar* pLayerTag);
	class CGameObject* Clone_GameObject(const _tchar* pPrototypeTag, void* pArg);
private:
	unordered_map<const _tchar*, CGameObject*> m_Prototypes;

private:
	_uint	m_iNumLevels = { 0 };
	unordered_map<const _tchar*, CLayer*>* m_pLayers = { nullptr };

private:
	CGameObject* Find_Prototype(const _tchar* pPrototypeTag);

public:
	void Free() override;
};

END