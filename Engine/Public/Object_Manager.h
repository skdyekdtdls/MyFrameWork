#pragma once

#include "Base.h"

BEGIN(Engine)
class CGameObject;
class CLayer;
class CObject_Manager final : public CBase
{
	DECLARE_SINGLETON(CObject_Manager)
public:
	CObject_Manager();
	~CObject_Manager() = default;

public:
	HRESULT Reserve_Containers(_uint iNumLevels);
	HRESULT Add_Prototype(const _tchar* pPrototypeTag, CGameObject* pGameObject);
	HRESULT Add_GameObject(_uint iLevelIndex, const _tchar* pPrototypeTag, const _tchar* pLayerTag, void* pArg);
	void Clear_LevelResources(_uint iLevelIndex);

private:
	unordered_map<const _tchar*, CGameObject*> m_Prototypes;

private:
	_uint	m_iNumLevels = { 0 };
	unordered_map<const _tchar*, CLayer*>* m_pLayers = { nullptr };
	typedef unordered_map<const _tchar*, CLayer*> LAYERS;

private:
	CGameObject* Find_Prototype(const _tchar* pPrototypeTag);
	CLayer* Find_Layer(_uint iLevelIndex, const _tchar* pLayerTag);

public:
	void Free() override;
};

END