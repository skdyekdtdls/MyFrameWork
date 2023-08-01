#pragma once

#include "Base.h"
#include "IObserver.h"

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

public: //Getter
	CGameObject* GetOwner();

	wstring GetComponentName() { return m_ComponentName; }

public: // Setter
	void SetComponentName(wstring Name) { m_ComponentName = Name; }

public:
	virtual HRESULT Initialize_Prototype(void);
	virtual HRESULT Initialize(void* pArg);
	virtual HRESULT Render() { return S_OK; }
protected:
	CGameObject* m_pOwner = { nullptr };
	ID3D11Device* m_pDevice = { nullptr };
	ID3D11DeviceContext* m_pContext = { nullptr };
	_bool	m_isCloned = { false };
	wstring m_ComponentName = L"";

public:
	virtual CComponent* Clone(void* pArg) = 0;
	virtual void Free(void);
};

END