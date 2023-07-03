#pragma once

#include "Composite.h"

BEGIN(Engine)
#ifdef _DEBUG
class CColliderSphere;
#endif

class ENGINE_DLL CGameObject : public CComposite
{
public:
	typedef struct tagCGameObjectDesc : public tagCCompositeDesc
	{
		tagCGameObjectDesc() : tagCCompositeDesc(), vPosition(_float4(0.f, 0.f, 0.f, 1.f)){};
		_float4 vPosition;
	}CGAMEOBJECT_DESC;
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
	virtual HRESULT Render() override;
	virtual void Tick(_double TimeDelta);
	virtual void Late_Tick(_double TimeDelta);

#ifdef _DEBUG
public:
	virtual _bool Picked(_Inout_ PICK_DESC& tPickDesc, const RAY& tMouseRay);
	_float GetPickSphereRadius();
#endif

	_float3 GetPosition();

public:
	virtual CGameObject* Clone(void* pArg) override = 0;
	virtual void Free() override;

protected:
	INFO m_tInfo;

#ifdef _DEBUG
	CColliderSphere* m_pPickCollider = { nullptr };
#endif // DEBUG
};

END