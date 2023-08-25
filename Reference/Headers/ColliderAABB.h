#pragma once
#include "Collider.h"

BEGIN(Engine)

class ENGINE_DLL CColliderAABB final : public CCollider
{
public:
	typedef struct tagCColliderAABBDesc : public tagCColliderDesc
	{
		tagCColliderAABBDesc() : tagCColliderDesc() {}
		_float3 Extents;
	}CCOLLIDER_AABB_DESC;
private:
	explicit CColliderAABB(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit CColliderAABB(const CColliderAABB& rhs);
	virtual ~CColliderAABB() = default;

public: // Getter
	BoundingBox* GetBoundingBox() const {
		return m_pBoundingBox;
	}

	BoundingBox* GetBoundingBox() {
		return m_pBoundingBox;
	}

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_fmatrix TransformMatrix) override;

	virtual _bool Intersect(CCollider* pOtherCollider, COLLISION_INFO& CollisionInfo) override;

#ifdef _DEBUG
	virtual HRESULT Render() override;
#endif

private:
	_float3 m_vExtents;
	BoundingBox* m_pBoundingBox_Origin = { nullptr };
	BoundingBox* m_pBoundingBox = { nullptr };

private:
	_bool CheckAABBCollision(CColliderAABB* pOtherCollider, COLLISION_INFO& CollisionInfo);
	_matrix Remove_Rotation(_fmatrix TransformMatrix);

public:
	static const _tchar* ProtoTag() { return L"Prototype_Component_CColliderAABB"; }
	static CColliderAABB* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};
END