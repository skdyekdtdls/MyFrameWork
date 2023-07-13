#include "ColliderAABB.h"
#include "ColliderSphere.h"
#include "Raycast.h"
CColliderAABB::CColliderAABB(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CCollider(pDevice, pContext)
{
}

CColliderAABB::CColliderAABB(const CColliderAABB& rhs)
	: CCollider(rhs)
	, m_pBoundingBox_Origin(new BoundingBox(*rhs.m_pBoundingBox_Origin))
	, m_pBoundingBox(new BoundingBox(*rhs.m_pBoundingBox))
	, m_vExtents(rhs.m_vExtents)
{
}

HRESULT CColliderAABB::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	m_pBoundingBox_Origin = new BoundingBox(_float3(0.f, 0.f, 0.f), _float3(0.5f, 0.5f, 0.5f));
	m_pBoundingBox = new BoundingBox(_float3(0.f, 0.f, 0.f), _float3(0.5f, 0.5f, 0.5f));
	m_eType = TYPE_AABB;

	return S_OK;
}

HRESULT CColliderAABB::Initialize(void* pArg)
{
	if (nullptr == pArg)
		return S_OK;

	__super::Initialize(pArg);
	CCOLLIDER_AABB_DESC tColliderAABBDesc = *(CCOLLIDER_AABB_DESC*)pArg;
	m_vExtents = tColliderAABBDesc.Extents;
	m_pBoundingBox_Origin->Center = tColliderAABBDesc.vCenter;
	m_pBoundingBox_Origin->Extents = tColliderAABBDesc.Extents;
	m_pBoundingBox->Center = tColliderAABBDesc.vCenter;
	m_pBoundingBox->Extents = tColliderAABBDesc.Extents;

	return S_OK;
}

void CColliderAABB::Tick(_fmatrix TransformMatrix)
{
	__super::Tick(TransformMatrix);

	m_pBoundingBox_Origin->Transform(*m_pBoundingBox, TransformMatrix);
}

_bool CColliderAABB::Intersect(CCollider* pOtherCollider)
{
	m_isColl = { false };
	switch (pOtherCollider->GetType())
	{
	case TYPE_SPHERE:
		m_isColl = m_pBoundingBox->Intersects(*static_cast<const CColliderSphere*>(pOtherCollider)->GetBoundingSphere());
		break;
	case TYPE_AABB:
		m_isColl = m_pBoundingBox->Intersects(*static_cast<const CColliderAABB*>(pOtherCollider)->GetBoundingBox());
		break;
	case TYPE_RAY:
	{
		const Raycast* pRaycast = static_cast<const Raycast*>(pOtherCollider);
		_float fLength;

		_vector vRayDir = pRaycast->GetRayDir();
		if (!XMVector4Equal(vRayDir, XMVectorZero()))
		{
			m_isColl = m_pBoundingBox->Intersects(pRaycast->GetRayOrigin(), vRayDir, fLength);
				if (m_isColl)
				{
					if (fLength >= pRaycast->GetLength())
						m_isColl = false;
				}
		}
	}
	break;
	}
	return m_isColl;
}

#ifdef _DEBUG
HRESULT CColliderAABB::Render()
{
	__super::SetupResources();
	__super::Begin();

	DX::Draw(GetBatch(), *m_pBoundingBox, IsColl() == true ? XMVectorSet(1.f, 0.f, 0.f, 1.f) : XMVectorSet(0.f, 1.f, 0.f, 1.f));

	__super::End();

	return S_OK;
}
#endif

CColliderAABB* CColliderAABB::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CColliderAABB* pInstance = new CColliderAABB(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created CColliderAABB");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CColliderAABB::Clone(void* pArg)
{
	CColliderAABB* pInstance = new CColliderAABB(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned CColliderAABB");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CColliderAABB::Free()
{
	__super::Free();

	Safe_Delete(m_pBoundingBox_Origin);
	Safe_Delete(m_pBoundingBox);
}