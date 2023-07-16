#include "Raycast.h"
#include "VIBuffer_Ray.h"
#include "PipeLine.h"
#include "DebugDraw.h"
#include "ColliderSphere.h"
#include "ColliderAABB.h"
Raycast::Raycast(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CCollider(pDevice, pContext)
{
}

Raycast::Raycast(const Raycast& rhs)
	: CCollider(rhs)
	, m_fLength(rhs.m_fLength)
{

}

HRESULT Raycast::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	m_eType = TYPE_RAY;

	return S_OK;
}

HRESULT Raycast::Initialize(void* pArg)
{
	if (nullptr == pArg)
		return S_OK;

	__super::Initialize(pArg);

	RAYCAST_DESC tRaycastDesc = *(RAYCAST_DESC*)pArg;
	m_fLength = tRaycastDesc.fLength;
	m_vCenter = tRaycastDesc.vCenter;

	return S_OK;
}

void Raycast::Tick(_fvector _vRayOrigin, _fvector _vRayDir)
{
	__super::Tick(XMMatrixIdentity());

	_vector vRayOrigin = _vRayOrigin + XMLoadFloat3(&m_vCenter);

	XMStoreFloat4(&m_tRay.vRayOrigin, vRayOrigin);
	XMStoreFloat4(&m_tRay.vRayDir, _vRayDir);
}

_bool Raycast::Intersect(CCollider* pOtherCollider, COLLISION_INFO& CollisionInfo)
{
	switch (pOtherCollider->GetType())
	{
	case TYPE_SPHERE:
		m_isColl = static_cast<CColliderSphere*>(pOtherCollider)->Intersect(this, CollisionInfo);
		break;
	case TYPE_AABB:
		m_isColl = static_cast<CColliderAABB*>(pOtherCollider)->Intersect(this, CollisionInfo);
		break;
	}

	return m_isColl;
}

#ifdef _DEBUG
HRESULT Raycast::Render()
{
	__super::SetupResources();
	__super::Begin();

	DX::DrawRay(GetBatch(), GetRayOrigin(), GetRayDir() * m_fLength, false
		, IsColl() == true ? XMVectorSet(1.f, 0.f, 0.f, 1.f) : XMVectorSet(0.f, 1.f, 0.f, 1.f));

	__super::End();
	return S_OK;
}
#endif

Raycast* Raycast::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	Raycast* pInstance = new Raycast(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created Raycast");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* Raycast::Clone(void* pArg)
{
	Raycast* pInstance = new Raycast(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned Raycast");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void Raycast::Free()
{
	__super::Free();
}