#pragma once

#include "Base.h"

BEGIN(Engine)
class CComponent;
class ENGINE_DLL CGameObject : public CBase
{
protected:
	CGameObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CGameObject(const CGameObject& rhs);
	virtual ~CGameObject() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_double TimeDelta);
	virtual void Late_Tick(_double TimeDelta);
	virtual HRESULT Render();

	HRESULT Add_Component(_uint iLevelIndex, const _tchar* pPrototypeTag, const _tchar* pComponentTag, _Inout_ CComponent** ppOut, void* pArg = nullptr);
protected:
	unordered_map<const _tchar*, CComponent*> m_Components;
	ID3D11Device* m_pDevice = { nullptr };
	ID3D11DeviceContext* m_pContext = { nullptr };

public:
	virtual CGameObject* Clone(void* pArg) = 0;
	void Free() override;
};

END