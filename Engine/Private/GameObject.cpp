#include "..\Public\GameObject.h"
#include "Transform.h"
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

	return S_OK;
}

void CGameObject::Tick(_double TimeDelta)
{
}

void CGameObject::Late_Tick(_double TimeDelta)
{
}

_bool CGameObject::Picked(PICK_DESC& tPickDesc, const RAY& tMouseRay)
{
	CTransform* pTransform = static_cast<CTransform*>(Get_Component(L"Com_Transform"));
	if (nullptr == pTransform)
		return false;

	_vector vRayOrigin = XMLoadFloat4(&tMouseRay.vRayOrigin);
	_vector vRayDir = XMLoadFloat4(&tMouseRay.vRayDir);

	vRayDir = XMVector3Normalize(vRayDir);

	_float3 vPosFloat3;

	XMStoreFloat3(&vPosFloat3, pTransform->Get_State(CTransform::STATE_POSITION));
	BoundingSphere sphere(vPosFloat3, 1.f);
	_float fDistance = FLT_MAX;
	sphere.Radius = 0.5f;

	if (sphere.Intersects(vRayOrigin, vRayDir, fDistance)) {
		if (tPickDesc.fDist > fDistance)
		{
			tPickDesc.fDist = fDistance;
			tPickDesc.vPickPos = vPosFloat3;
			tPickDesc.pPickedObject = this;
		}
		return true;
	}
	
	return false;
}

void CGameObject::Free()
{
	__super::Free();
}
