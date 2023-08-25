#include "ColliderSphere.h"
#include "ColliderAABB.h"

CColliderSphere::CColliderSphere(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CCollider(pDevice, pContext)
{
}

CColliderSphere::CColliderSphere(const CColliderSphere& rhs)
	: CCollider(rhs)
	, m_pBoudingSphere_Origin(new BoundingSphere(*rhs.m_pBoudingSphere_Origin))
	, m_pBoudingSphere(new BoundingSphere(*rhs.m_pBoudingSphere))
{

}

HRESULT CColliderSphere::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	m_pBoudingSphere_Origin = new BoundingSphere(_float3(0.f, 0.f, 0.f), 0.5f);
	m_pBoudingSphere = new BoundingSphere(_float3(0.f, 0.f, 0.f), 0.5f);
	m_eType = TYPE_SPHERE;

	return S_OK;
}

HRESULT CColliderSphere::Initialize(void* pArg)
{
	__super::Initialize(pArg);
	CCOLLIDER_SPHERE_DESC tColliderSphereDesc = *(CCOLLIDER_SPHERE_DESC*)pArg;
	m_pBoudingSphere_Origin->Radius = tColliderSphereDesc.fRadius;
	m_pBoudingSphere_Origin->Center = tColliderSphereDesc.vCenter;

	return S_OK;
}

void CColliderSphere::Tick(_fmatrix TransformMatrix)
{
	__super::Tick(TransformMatrix);

	m_pBoudingSphere_Origin->Transform(*m_pBoudingSphere, TransformMatrix);
}

_bool CColliderSphere::IntersectRay(_float& fDist, const RAY& tMouseRay)
{
	_vector vRayOrigin = XMLoadFloat4(&tMouseRay.vRayOrigin);
	_vector vRayDir = XMLoadFloat4(&tMouseRay.vRayDir);
	vRayDir = XMVector3Normalize(vRayDir);

	if (m_pBoudingSphere->Intersects(vRayOrigin, vRayDir, fDist)) {
		return true;
	}

	return false;
}

_bool CColliderSphere::Intersect(CCollider* pOtherCollider, COLLISION_INFO& CollisionInfo)
{
	switch (pOtherCollider->GetType())
	{
	case TYPE_SPHERE:
		m_isColl = m_pBoudingSphere->Intersects(*static_cast<CColliderSphere*>(pOtherCollider)->GetBoundingSphere());
		break;
	case TYPE_AABB:
		m_isColl = m_pBoudingSphere->Intersects(*static_cast<CColliderAABB*>(pOtherCollider)->GetBoundingBox());
		break;
	}
	return m_isColl;
}

#ifdef _DEBUG
HRESULT CColliderSphere::Render()
{
	__super::SetupResources();

	__super::Begin();

	DX::Draw(GetBatch(), *m_pBoudingSphere, IsColl() == true ? XMVectorSet(1.f, 0.f, 0.f, 1.f) : XMVectorSet(0.f, 1.f, 0.f, 1.f));

	__super::End();

	return S_OK;
}
void CColliderSphere::SetPickRadius(_float fRadius)
{
	m_pBoudingSphere_Origin->Radius = fRadius;
}
#endif

CColliderSphere* CColliderSphere::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CColliderSphere* pInstance = new CColliderSphere(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created CColliderSphere");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CColliderSphere::Clone(void* pArg)
{
	CColliderSphere* pInstance = new CColliderSphere(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned CColliderSphere");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CColliderSphere::Free()
{
	__super::Free();

	Safe_Delete(m_pBoudingSphere_Origin);
	Safe_Delete(m_pBoudingSphere);
}