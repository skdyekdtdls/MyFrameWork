#include "ColliderSphere.h"

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
	m_eTYPE = TYPE_SPHERE;

	return S_OK;
}

HRESULT CColliderSphere::Initialize(void* pArg)
{
	__super::Initialize(pArg);
	COLLIDER_SPHERE_DESC tColliderSphereDesc = *(COLLIDER_SPHERE_DESC*)pArg;
	m_pBoudingSphere_Origin->Radius = tColliderSphereDesc.fRadius;
	m_pBoudingSphere_Origin->Center = tColliderSphereDesc.vCenter;

	return S_OK;
}

void CColliderSphere::Tick(_fmatrix TransformMatrix)
{
	__super::Tick(TransformMatrix);

	m_pBoudingSphere_Origin->Transform(*m_pBoudingSphere, TransformMatrix);
}

HRESULT CColliderSphere::Render()
{
	SetupResources();

	__super::Begin();

	DX::Draw(GetBatch(), *m_pBoudingSphere, IsColl() == true ? XMVectorSet(1.f, 0.f, 0.f, 1.f) : XMVectorSet(0.f, 1.f, 0.f, 1.f));

	__super::End();

	return S_OK;
}

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