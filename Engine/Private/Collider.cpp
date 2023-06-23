#include "Collider.h"
#ifdef _DEBUG
#include "PipeLine.h"
#endif
#include "ColliderSphere.h"

CCollider::CCollider(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CComponent(pDevice, pContext)
{
}

CCollider::CCollider(const CCollider& rhs)
	: CComponent(rhs)
#ifdef _DEBUG
	, m_pBatch(rhs.m_pBatch)
	, m_pEffect(rhs.m_pEffect)
	, m_pInputLayout(rhs.m_pInputLayout)
#endif
{
#ifdef _DEBUG
	Safe_AddRef(m_pInputLayout);
#endif
}

HRESULT CCollider::Initialize_Prototype()
{
#ifdef _DEBUG
	m_pBatch = new PrimitiveBatch<DirectX::VertexPositionColor>(m_pContext);
	m_pEffect = new BasicEffect(m_pDevice);

	m_pEffect->SetVertexColorEnabled(true);

	const void* pShaderByteCode;
	size_t iShaderCodeLength;

	m_pEffect->GetVertexShaderBytecode(&pShaderByteCode, &iShaderCodeLength);

	FAILED_CHECK_RETURN(m_pDevice->CreateInputLayout(VertexPositionColor::InputElements
		, VertexPositionColor::InputElementCount
		, pShaderByteCode
		, iShaderCodeLength, &m_pInputLayout), E_FAIL);
#endif

	return S_OK;
}

HRESULT CCollider::Initialize(void* pArg)
{

	return S_OK;
}

void CCollider::Tick(_fmatrix TransformMatrix)
{
	
}

#ifdef _DEBUG
void CCollider::SetupResources()
{
	NULL_CHECK(m_pBatch, E_FAIL);

	m_pEffect->SetWorld(XMMatrixIdentity());

	CPipeLine* pPipeLine = CPipeLine::GetInstance();
	Safe_AddRef(pPipeLine);

	_float4x4 ViewMatrix = pPipeLine->Get_TransformFloat4x4(CPipeLine::D3DTS_VIEW);
	m_pEffect->SetView(XMLoadFloat4x4(&ViewMatrix));

	_float4x4 ProjMatrix = pPipeLine->Get_TransformFloat4x4(CPipeLine::D3DTS_PROJ);
	m_pEffect->SetProjection(XMLoadFloat4x4(&ProjMatrix));

	Safe_Release(pPipeLine);

	m_pEffect->Apply(m_pContext);

	m_pContext->IASetInputLayout(m_pInputLayout);
}

void CCollider::Begin()
{
	m_pBatch->Begin();
}

void CCollider::End()
{
	m_pBatch->End();
}
#endif

_bool CCollider::Intersect(const CCollider* pCollider)
{
	m_isColl = { false };

	switch (m_eTYPE)
	{
	case Engine::CCollider::TYPE_SPHERE:
		m_isColl = IntersectSphere(pCollider);
		break;
	case Engine::CCollider::TYPE_AABB:
		break;
	case Engine::CCollider::TYPE_OBB:
		break;
	case Engine::CCollider::TYPE_END:
		break;
	default:
		break;
	}

	return m_isColl;
}

_bool CCollider::IntersectSphere(const CCollider* pCollider)
{
	BoundingSphere* pMyBoudingSphere = static_cast<const CColliderSphere*>(this)->GetBoundingSphere();
	_bool bResult = { false };

	switch (pCollider->m_eTYPE)
	{
	case Engine::CCollider::TYPE_SPHERE:
	{
		BoundingSphere* pOtherBoudingSphere = static_cast<const CColliderSphere*>(pCollider)->GetBoundingSphere();
		bResult = pMyBoudingSphere->Intersects(*pOtherBoudingSphere);
	}
	break;
	case Engine::CCollider::TYPE_AABB:
		break;
	case Engine::CCollider::TYPE_OBB:
		break;
	case Engine::CCollider::TYPE_END:
		break;
	default:
		break;
	}

	return bResult;
}

void CCollider::Free()
{
	__super::Free();

#ifdef _DEBUG
	if (false == m_isCloned)
	{
		Safe_Delete(m_pBatch);
		Safe_Delete(m_pEffect);
	}
	Safe_Release(m_pInputLayout);
#endif
}
