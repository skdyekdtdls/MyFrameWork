#include "..\Public\GameObject.h"
#include "Transform.h"
#include "ColliderSphere.h"

CGameObject::CGameObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CComposite(pDevice, pContext)
	, m_tInfo()
{
}

CGameObject::CGameObject(const CGameObject& rhs)
	: CComposite(rhs)
	, m_tInfo()
{
}

HRESULT CGameObject::Initialize_Prototype()
{
	FAILED_CHECK_RETURN(__super::Initialize_Prototype(), E_FAIL);

	return S_OK;
}

HRESULT CGameObject::Initialize(void* pArg)
{
	FAILED_CHECK_RETURN(__super::Initialize(pArg), E_FAIL);

#ifdef _DEBUG
	CColliderSphere::COLLIDER_SPHERE_DESC tColliderSphereDesc;
	tColliderSphereDesc.fRadius = { 0.5f };
	tColliderSphereDesc.vCenter = { _float3(0.f, tColliderSphereDesc.fRadius * 1.f, 0.f) };
	FAILED_CHECK_RETURN(__super::Add_Component(1, CColliderSphere::ProtoTag()
		, L"Com_PickCollider", (CComponent**)&m_pPickCollider, &tColliderSphereDesc), E_FAIL);
#endif //  _DEBUG

	return S_OK;
}

HRESULT CGameObject::Render()
{
#ifdef _DEBUG
	m_pPickCollider->Render();
#endif // _DEBUG

	return S_OK;
}

void CGameObject::Tick(_double TimeDelta)
{
#ifdef _DEBUG
	if (!HasTransformCom())
		return;
	CTransform* pTransform = static_cast<CTransform*>(Get_Component(L"Com_Transform"));

	NULL_CHECK(m_pPickCollider)
	m_pPickCollider->Tick(pTransform->Get_WorldMatrix());
#endif
}

void CGameObject::Late_Tick(_double TimeDelta)
{
}

#ifdef _DEBUG
_bool CGameObject::Picked(PICK_DESC& tPickDesc, const RAY& tMouseRay)
{
	if (!HasTransformCom())
		return false;
	_float3 vPosFloat3 = GetPosition();

	_float fDistance = { FLT_MAX };
	if (m_pPickCollider->IntersectRay(fDistance, tMouseRay))
	{
		tPickDesc.fDist = fDistance; 
		tPickDesc.vPickPos = *(_float4*)&vPosFloat3;
		tPickDesc.pPickedObject = this;
		return true;
	}
	
	return false;
}
#endif


#ifdef _DEBUG
_float CGameObject::GetPickSphereRadius()
{
	return m_pPickCollider->GetBoundingSphere()->Radius;
}
#endif

_float3 CGameObject::GetPosition()
{
	if (!HasTransformCom())
		return _float3(0.f, 0.f, 0.f);
	CTransform* pTransform = static_cast<CTransform*>(Get_Component(L"Com_Transform"));
	_float3 vPos;
	XMStoreFloat3(&vPos, pTransform->Get_State(CTransform::STATE_POSITION));

	return vPos;
}


void CGameObject::Free()
{
	__super::Free();
#ifdef _DEBUG
	Safe_Release(m_pPickCollider);
#endif // _DEBUG

}
