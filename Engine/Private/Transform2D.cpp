#include "Transform2D.h"

CTransform2D::CTransform2D(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CComponent(pDevice, pContext)
	, m_TransformDesc(CTRANSFORM2D_DESC())
{
	_matrix matIdentity = XMMatrixIdentity();
	XMStoreFloat4x4(&m_WorldMatrix, matIdentity);
}

CTransform2D::CTransform2D(const CTransform2D& rhs)
	: CComponent(rhs)
	, m_WorldMatrix(rhs.m_WorldMatrix)
	, m_TransformDesc(rhs.m_TransformDesc)
{

}

HRESULT CTransform2D::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CTransform2D::Initialize(void* pArg)
{
	__super::Initialize(pArg);
	return S_OK;
}

void CTransform2D::Go_Up(const _double& TimeDelta)
{
	_vector vPos = GetPosition();
	vPos.m128_f32[1] -= m_TransformDesc.SpeedPerSec * TimeDelta;
	SetPosition(vPos);
}

void CTransform2D::Go_Down(const _double& TimeDelta)
{
	_vector vPos = GetPosition();
	vPos.m128_f32[1] += m_TransformDesc.SpeedPerSec * TimeDelta;
	SetPosition(vPos);
}

void CTransform2D::Go_Left(const _double& TimeDelta)
{
	_vector vPos = GetPosition();
	vPos.m128_f32[0] -= m_TransformDesc.SpeedPerSec * TimeDelta;
	SetPosition(vPos);
}

void CTransform2D::Go_Right(const _double& TimeDelta)
{
	_vector vPos = GetPosition();
	vPos.m128_f32[0] += m_TransformDesc.SpeedPerSec * TimeDelta;
	SetPosition(vPos);
}

void CTransform2D::Jump(_float TimeElapse, _float fPower, _float fGravity)
{
	m_WorldMatrix._42 += (fGravity * TimeElapse * TimeElapse) - fPower * TimeElapse;
}

void CTransform2D::Chase(const _double& TimeDelta, _fvector vTargetPosition, _float fMinDistance)
{
	_vector		vPosition = GetPosition();

	_vector		vDir = vTargetPosition - vPosition;

	if (fMinDistance < XMVectorGetX(XMVector3Length(vDir)))
		vPosition += XMVector3Normalize(vDir) * m_TransformDesc.SpeedPerSec * TimeDelta;

	SetPosition(vPosition);
}

void CTransform2D::Rotation(_float fDegree)
{
	_matrix matRotZ;
	_matrix matScale;
	_float2 fScale = GetScale();
	matRotZ = XMMatrixRotationZ(XMConvertToRadians(fDegree));
	matScale = XMMatrixScaling(fScale.x, fScale.y, 1.f);
	XMStoreFloat4x4(&m_WorldMatrix,  matRotZ * matScale);
}

void CTransform2D::TurnCCW(const _double& TimeDelta)
{
	_float3 vRight = *(_float3*)&m_WorldMatrix._11;
	_float3 vUp = *(_float3*)&m_WorldMatrix._21;

	_matrix RotationMatrix = XMMatrixRotationZ(XMConvertToRadians(-m_TransformDesc.RotationPerSec) * TimeDelta);

	XMVECTOR tempRight = XMVector3TransformNormal(XMLoadFloat3(&vRight), RotationMatrix);
	XMVECTOR tempUp = XMVector3TransformNormal(XMLoadFloat3(&vUp), RotationMatrix);

	XMStoreFloat3(&vRight, tempRight);
	XMStoreFloat3(&vUp, tempUp);

	memcpy(&m_WorldMatrix._11, &vRight, sizeof(_float3));
	memcpy(&m_WorldMatrix._21, &vUp, sizeof(_float3));
}

void CTransform2D::TurnCW(const _double& TimeDelta)
{
	_float3 vRight = *(_float3*)&m_WorldMatrix._11;
	_float3 vUp = *(_float3*)&m_WorldMatrix._21;

	_matrix RotationMatrix = XMMatrixRotationZ(XMConvertToRadians(m_TransformDesc.RotationPerSec) * TimeDelta);

	XMVECTOR tempRight = XMVector3TransformNormal(XMLoadFloat3(&vRight), RotationMatrix);
	XMVECTOR tempUp = XMVector3TransformNormal(XMLoadFloat3(&vUp), RotationMatrix);

	XMStoreFloat3(&vRight, tempRight);
	XMStoreFloat3(&vUp, tempUp);

	memcpy(&m_WorldMatrix._11, &vRight, sizeof(_float3));
	memcpy(&m_WorldMatrix._21, &vUp, sizeof(_float3));
}

void CTransform2D::Scaled(const _float2& vScale)
{
	_float3 vRight = *(_float3*)&m_WorldMatrix._11;
	_float3 vUp = *(_float3*)&m_WorldMatrix._21;

	XMStoreFloat3(&vRight, XMVector3Normalize(XMLoadFloat3(&vRight)) * vScale.x);
	XMStoreFloat3(&vUp, XMVector3Normalize(XMLoadFloat3(&vUp)) * vScale.y);

	memcpy(&m_WorldMatrix._11, &vRight, sizeof(_float3));
	memcpy(&m_WorldMatrix._21, &vUp, sizeof(_float3));
}

void CTransform2D::SetMoveSpeed(_float fMoveSpeed)
{
	m_TransformDesc.SpeedPerSec = fMoveSpeed;
}

void CTransform2D::SetRotationSpeed(_float fRotationSpeed)
{
	m_TransformDesc.RotationPerSec = fRotationSpeed;
}

_float CTransform2D::Get_AulerDegree()
{
	float roll; // z 축에 대한 회전

	roll = std::asin(m_WorldMatrix.m[1][0]);

	// roll 값을 degree로 변환
	roll = XMConvertToDegrees(roll);

	// Adjust the range from [-180, 180] to [0, 360]
	if (roll < 0) roll += 360.0f;

	// Adjust to 360 - roll value
	roll = 360.f - roll;

	if (roll > 359.9f) roll = 0.f;

	return roll;
}

_float2 CTransform2D::GetScale()
{
	_float2 fSize;
	_vector vRight = XMLoadFloat3((_float3*)&m_WorldMatrix._11);
	_vector vUp = XMLoadFloat3((_float3*)&m_WorldMatrix._21);
	fSize.x = XMVectorGetX(XMVector3Length(vRight));
	fSize.y = XMVectorGetX(XMVector3Length(vUp));
	return fSize;
}

_vector CTransform2D::GetPosition()
{
	_float2 vPos = _float2(m_WorldMatrix._41, m_WorldMatrix._42);
	return XMLoadFloat2(&vPos);
}

void CTransform2D::SetPosition(_fvector vPosition)
{
	m_WorldMatrix._41 = vPosition.m128_f32[0];
	m_WorldMatrix._42 = vPosition.m128_f32[1];
}


CTransform2D* CTransform2D::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CTransform2D* pInstance = new CTransform2D(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created CTransform2D");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CTransform2D::Clone(void* pArg)
{
	CTransform2D* pInstance = new CTransform2D(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned CTransform2D");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTransform2D::Free()
{
	__super::Free();
}

