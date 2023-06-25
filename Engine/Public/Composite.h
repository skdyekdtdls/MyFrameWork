#pragma once

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CComposite : public CComponent
{
protected:
	explicit CComposite(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit CComposite(const CComposite& rhs);
	virtual ~CComposite() = default;

public:
	HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_double TimeDelta);
	virtual void Late_Tick(_double TimeDelta);
	virtual HRESULT Render();


	HRESULT Add_Component(_uint iLevelIndex, const _tchar* pPrototypeTag, const _tchar* pComponentTag, _Inout_ CComponent** ppOut, void* pArg = nullptr);
	CComponent* Get_Component(const _tchar* pComponent);
	_bool HasTransformCom();
private:
	unordered_map<const _tchar*, CComponent*>	m_Components;

public:
	virtual CComponent* Clone(void* pArg) override = 0;
	virtual void Free(void) override;
};

END