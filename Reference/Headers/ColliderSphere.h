
#pragma once
#include "Collider.h"

BEGIN(Engine)

class ENGINE_DLL CColliderSphere final : public CCollider
{
public:
	typedef struct tagColliderSphereDesc : public tagCColliderDesc {
		tagColliderSphereDesc() : tagCColliderDesc(), fRadius(1.f) {};
		_float fRadius;
	}CCOLLIDER_SPHERE_DESC;
private:
	explicit CColliderSphere(ID3D11Device * pDevice, ID3D11DeviceContext * pContext);
	explicit CColliderSphere(const CColliderSphere& rhs);
	virtual ~CColliderSphere() = default;

public:
	BoundingSphere* GetBoundingSphere() const {
		return m_pBoudingSphere;
	}

public:
	HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_fmatrix TransformMatrix) override;

	_bool IntersectRay(_float& fDist, const RAY& tMouseRay);

#ifdef _DEBUG
	virtual HRESULT Render() override;
#endif

private:
	_float m_fRadius;
	BoundingSphere* m_pBoudingSphere_Origin = { nullptr };
	BoundingSphere* m_pBoudingSphere = { nullptr };

public:
	static const _tchar* ProtoTag() { return L"Prototype_Component_CColliderSphere"; }
	static CColliderSphere* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END
