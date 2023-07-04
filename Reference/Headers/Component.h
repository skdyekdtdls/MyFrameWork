#pragma once

#include "Base.h"

BEGIN(Engine)
class CGameObject;
class ENGINE_DLL CComponent abstract : public CBase
{
public:
	typedef struct tagComponentDesc
	{
		tagComponentDesc() : pOwner(nullptr) {}

		CGameObject* pOwner;
	}COMPONENT_DESC;

protected:
	CComponent() = default;
	CComponent(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CComponent(const CComponent & rhs);
	virtual ~CComponent(void) = default;

public:
	virtual HRESULT Initialize_Prototype(void);
	virtual HRESULT Initialize(void* pArg);

protected:
	CGameObject* m_pOwner = { nullptr };
	ID3D11Device* m_pDevice = { nullptr };
	ID3D11DeviceContext* m_pContext = { nullptr };
	_bool	m_isCloned = { false };

public:
	virtual CComponent* Clone(void* pArg) = 0;
	virtual void Free(void);
};

END