#pragma once

#include "Composite.h"
#include "Collider.h"
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

public: // Getter
	string Get_Name()
	{
		return TO_STR(m_tInfo.wstrName);
	}
	_float3 GetPosition();

	virtual _bool GetDead() { return m_bDead; }

	wstring GetLayerName() {
		return m_LayerName;
	}

public: // Setter
	void SetLayerName(wstring LayerName) { m_LayerName = LayerName; }
	virtual void SetDead() { m_bDead = true; }
	INFO GetInfo() { return m_tInfo; }

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual HRESULT Render() override;
	virtual void Tick(_double TimeDelta);
	virtual void Late_Tick(_double TimeDelta);

	virtual void OnCollision(CCollider::COLLISION_INFO tCollisionInfo, _double TimeDelta);

public:
	virtual _bool Picked(_Inout_ PICK_DESC& tPickDesc, const RAY& tMouseRay);

#ifdef _DEBUG
	_float GetPickSphereRadius();
	void ReleaseFreePickCollider();
	void SetPickRadius(_float fRadius);
#endif

public:
	virtual CGameObject* Clone(void* pArg) override = 0;
	virtual void Free() override;

protected:
	wstring m_LayerName;
	INFO m_tInfo;
	_bool m_bDead;
#ifdef _DEBUG
	CColliderSphere* m_pPickCollider = { nullptr };
#endif // DEBUG
};

END