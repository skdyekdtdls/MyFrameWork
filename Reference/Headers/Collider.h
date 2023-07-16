
#pragma once
#include "Component.h"
#ifdef _DEBUG
#include "DebugDraw.h"
#endif
#include "CollisionMgr.h"

BEGIN(Engine)
class ENGINE_DLL CCollider abstract : public CComponent
{
public:
	enum TYPE { TYPE_SPHERE, TYPE_AABB, TYPE_OBB, TYPE_RAY, TYPE_END };

	typedef struct tagCColliderDesc : public tagComponentDesc
	{
		tagCColliderDesc() : tagComponentDesc(), vCenter(_float3()) {};
		_float3 vCenter;
	}CCOLLIDER_DESC;

	typedef struct tagCollisionInfo
	{
		CGameObject* pOtherGameObject;
		string OtherGameObejctName;
		wstring OtherGameObjectLayerName;

		CCollider* pOtherCollider;
		wstring OtherCollName;

		CCollider* pMyCollider;
		wstring MyCollName;

		_float3 vOverLapVector;

	}COLLISION_INFO;

protected:
	explicit CCollider(ID3D11Device * pDevice, ID3D11DeviceContext * pContext);
	explicit CCollider(const CCollider& rhs);
	virtual ~CCollider() = default;

public: // Getter
	const TYPE& GetType() const {
		return m_eType;
	}

public: // Setter
	void SetIsColl(_bool isColl) {
		m_isColl = isColl;
	}
public:
	_bool IsColl() const {
		return m_isColl;
	}

public:
	HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_fmatrix TransformMatrix);
	void Add_ColliderGroup(COLL_GROUP eCollGroup);

#ifdef _DEBUG
	PrimitiveBatch<DirectX::VertexPositionColor>* GetBatch() {
		return m_pBatch;
	}
	virtual HRESULT Render() = 0;
	void SetupResources();
	void Begin();
	void End();
#endif

public:
	void OnCollision(COLLISION_INFO tCollisionInfo, _double TimeDelta);
	virtual _bool Intersect(CCollider* pCollider, COLLISION_INFO& CollisionInfo) = 0;

protected:
	TYPE			m_eType = { TYPE_END };

#ifdef _DEBUG
private:
	PrimitiveBatch<DirectX::VertexPositionColor>*	m_pBatch = { nullptr };
	BasicEffect*									m_pEffect = { nullptr };
	ID3D11InputLayout*								m_pInputLayout = { nullptr };
#endif
protected:
	_bool m_isColl = { false };

public:
	virtual void Free() override;
};

END
