#pragma once

#include "Base.h"
#include "ISerializable.h"

BEGIN(Engine)
class CGameObject;
class ENGINE_DLL CLayer final : public CBase, public ISerializable
{
private:
	CLayer();
	CLayer(const CLayer& rhs) = delete;
	CLayer& operator=(const CLayer& rhs) = delete;
	~CLayer() = default;

public:
	virtual void Save(HANDLE hFile, DWORD& dwByte) override;
	virtual void Load(HANDLE hFile, DWORD& dwByte, _uint iLevelIndex) override;

public:
	const _tchar* GetLayerName() const	{
		return m_tInfo.wstrName.c_str();
	}

	void SetLayerName(wstring wstrLayerName) {
		m_tInfo.wstrName = wstrLayerName.c_str();
	}
	const list<CGameObject*>& GetGameObjects() {
		return m_pGameObjects;
	}

	auto Begin() {
		return m_pGameObjects.begin();
	}

	auto End() {
		return m_pGameObjects.end();
	}

public:
	HRESULT Add_GameObject(CGameObject * pGameObject);
	HRESULT Delete_GameObject(string strName);
	void Tick(_double TimeDelta);
	void Late_Tick(_double TimeDelta);

	CGameObject* FindByName(string strName);
private:
	void ReleaseGameObject();

private:
	INFO			m_tInfo;
	list<CGameObject*>	m_pGameObjects;

public:
	static CLayer* Create(wstring wstrLayerName);
	void Free() override;
};

END