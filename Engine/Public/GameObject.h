#pragma once

#include "Composite.h"

BEGIN(Engine)

class ENGINE_DLL CGameObject : public CComposite
{
protected:
	CGameObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CGameObject(const CGameObject& rhs);
	virtual ~CGameObject() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_double TimeDelta);
	virtual void Late_Tick(_double TimeDelta);

public:
	virtual CGameObject* Clone(void* pArg) override = 0;
	virtual void Free() override;
};

END