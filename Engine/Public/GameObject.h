#pragma once

#include "Composite.h"

BEGIN(Engine)

class ENGINE_DLL CGameObject : public CComposite
{
public:
	typedef struct tagCloneDesc
	{
		_float3 vPosition;
	}CLONE_DESC;
protected:
	CGameObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CGameObject(const CGameObject& rhs);
	virtual ~CGameObject() = default;

public:
	string Get_Name()
	{
		return TO_STR(m_tInfo.wstrName);
	}

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_double TimeDelta);
	virtual void Late_Tick(_double TimeDelta);

	virtual _bool Picked(_Inout_ PICK_DESC& tPickDesc, const RAY& tMouseRay);

public:
	virtual CGameObject* Clone(void* pArg) override = 0;
	virtual void Free() override;

protected:
	INFO m_tInfo;
};

END