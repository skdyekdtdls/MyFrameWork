#pragma once

#include "Base.h"

BEGIN(Engine)
class CGameObject;
class ENGINE_DLL CLayer final : public CBase
{
private:
	CLayer();
	~CLayer() = default;

public:
	HRESULT Add_GameObject(CGameObject * pGameObject);
	void Tick(_double TimeDelta);
	void Late_Tick(_double TimeDelta);

private:
	list<CGameObject*>	m_pGameObjects;
public:
	static CLayer* Create();
	void Free() override;
};

END