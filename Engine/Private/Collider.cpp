#include "Collider.h"
#ifdef _DEBUG
#include "PipeLine.h"
#endif
#include "ColliderSphere.h"
#include "GameObject.h"

CCollider::CCollider(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CComponent(pDevice, pContext)
{
}

CCollider::CCollider(const CCollider& rhs)
	: CComponent(rhs)
	, m_eType(rhs.m_eType)
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
	if (nullptr == pArg)
		return S_OK;

	__super::Initialize(pArg);

	return S_OK;
}

void CCollider::Tick(_fmatrix TransformMatrix)
{
	m_isColl = { false };
}

void CCollider::Add_ColliderGroup(COLL_GROUP eCollGroup)
{
	CollisionMgr::GetInstance()->Add_ColliderGroup(this, eCollGroup);
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

void CCollider::OnCollision(COLLISION_INFO tCollisionInfo, _double TimeDelta)
{
	if (nullptr == m_pOwner)
		return;

	m_pOwner->OnCollision(tCollisionInfo, TimeDelta);
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
