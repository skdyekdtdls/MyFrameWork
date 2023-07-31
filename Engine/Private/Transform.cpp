#include "..\Public\Transform.h"
#include "Navigation.h"
#include "GameObject.h"
#include "Model.h"

CTransform::CTransform(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CComponent(pDevice, pContext)
	, m_TransformDesc(0.0, 0.0)
	, m_eCurDirection(DIRECTION::DIR_N)
{
	XMStoreFloat4x4(&m_PrevWorldMatrix, XMMatrixIdentity());
	XMStoreFloat4x4(&m_WorldMatrix, XMMatrixIdentity());
}

CTransform::CTransform(const CTransform& rhs)
	: CComponent(rhs)
	, m_TransformDesc(rhs.m_TransformDesc)
	, m_eCurDirection(rhs.m_eCurDirection)
	, m_WorldMatrix(rhs.m_WorldMatrix)
	, m_PrevWorldMatrix(rhs.m_PrevWorldMatrix)
{
}

void CTransform::Save(HANDLE hFile, DWORD& dwByte)
{
	WriteVoid(&m_WorldMatrix, sizeof(_float4x4));
	WriteVoid(&m_TransformDesc.RotationPerSec, sizeof(_double));
	WriteVoid(&m_TransformDesc.SpeedPerSec, sizeof(_double));
}

void CTransform::Load(HANDLE hFile, DWORD& dwByte, _uint iLevelIndex)
{
	ReadVoid(&m_WorldMatrix, sizeof(_float4x4));
	ReadVoid(&m_TransformDesc.RotationPerSec, sizeof(_double));
	ReadVoid(&m_TransformDesc.SpeedPerSec, sizeof(_double));
}

_float3 CTransform::Get_Scaled()
{
	return _float3(XMVectorGetX(XMVector3Length(XMLoadFloat4x4(&m_WorldMatrix).r[STATE_RIGHT])),
		XMVectorGetX(XMVector3Length(XMLoadFloat4x4(&m_WorldMatrix).r[STATE_UP])),
		XMVectorGetX(XMVector3Length(XMLoadFloat4x4(&m_WorldMatrix).r[STATE_LOOK])));
}

_float3 CTransform::Get_AulerDegree()
{
	// YZX순서라고 가정
	_float3 euler;

	float yaw, pitch, roll; // y, x, z 축에 대한 회전

	roll = std::asin(m_WorldMatrix.m[1][0]);
	if (std::cos(roll) != 0) {
		pitch = std::atan2(-m_WorldMatrix.m[2][0], m_WorldMatrix.m[0][0]);
		yaw = std::atan2(-m_WorldMatrix.m[1][2], m_WorldMatrix.m[1][1]);
	}
	else {
		pitch = 0;
		yaw = std::atan2(m_WorldMatrix.m[2][1], -m_WorldMatrix.m[2][2]);
	}
	euler.x = XMConvertToDegrees(yaw);
	euler.y = XMConvertToDegrees(pitch);
	euler.z = XMConvertToDegrees(roll);

	//// Adjust the range from [-180, 180] to [0, 360]
	if (euler.x < 0) euler.x += 360.0f;
	if (euler.y < 0) euler.y += 360.0f;
	if (euler.z < 0) euler.z += 360.0f;

	euler.x = 360.f - euler.x;
	euler.y = 360.f - euler.y;
	euler.z = 360.f - euler.z;

	if (euler.x > 359.9f) euler.x = 0.f;
	if (euler.y > 359.9f) euler.y = 0.f;
	if (euler.z > 359.9f) euler.z = 0.f;

	return euler;
}

void CTransform::Set_State(STATE _eState, _fvector _vState)
{
	_float4		vState;

	XMStoreFloat4(&vState, _vState);

	memcpy(&m_PrevWorldMatrix.m[_eState][0], &m_WorldMatrix.m[_eState][0], sizeof vState);
	memcpy(&m_WorldMatrix.m[_eState][0], &vState, sizeof vState);
}

HRESULT CTransform::Initialize_Prototype()
{
	XMStoreFloat4x4(&m_WorldMatrix, XMMatrixIdentity());

	return S_OK;
}

HRESULT CTransform::Initialize(void* pArg)
{
	__super::Initialize(pArg);

	if (nullptr != pArg)
		memmove(&m_TransformDesc, pArg, sizeof m_TransformDesc);

	return S_OK;
}



void CTransform::Go_Straight(_double TimeDelta, CNavigation* pNavigation)
{
	_vector		vPosition = Get_State(STATE_POSITION);
	_vector		vLook = Get_State(STATE_LOOK);

	_vector		vNextPosition = vPosition + XMVector3Normalize(vLook) * m_TransformDesc.SpeedPerSec * TimeDelta;
	_vector		vDir = XMVector3Normalize(vLook) * m_TransformDesc.SpeedPerSec;
	_bool		isMove = true; // 기본적으로 플레이어는 이동 상태로 가정합니다.

	if (pNavigation != nullptr)
	{
		if (true == (isMove = pNavigation->is_Move(vNextPosition))) // NonSliding
		{
			vPosition = vNextPosition; // 위치를 업데이트합니다.
			//vPosition -= XMVector3Normalize(XMLoadFloat3(&vContactNormal)) * 0.13f; // 벽에 갇히지 않게 밀어냄
		}
		else
		{
			while (false == isMove)
			{
				vNextPosition = vPosition;
				vDir *= 0.6f;
				_float3 vContactNormal = pNavigation->ContactNormal(); // 충돌 법선을 가져옵니다.
				_vector vSlidingVector = pNavigation->GetSlidingVector(vDir, XMLoadFloat3(&vContactNormal));
				vNextPosition += vSlidingVector * m_TransformDesc.SpeedPerSec * TimeDelta;

				isMove = pNavigation->is_Move(vNextPosition);
			}

			vPosition = vNextPosition; // 위치를 업데이트합니다.
		}
	}
	else
		vPosition = vNextPosition;

	// 이동 가능한 상태라면, 새로 계산한 위치를 설정합니다.
	if (true == isMove)
	{
		Set_State(STATE_POSITION, vPosition);
	}
}

void CTransform::Go_Backward(_double TimeDelta, CNavigation* pNavigation)
{
	_vector		vPosition = Get_State(STATE_POSITION);
	_vector		vLook = Get_State(STATE_LOOK);

	vPosition -= XMVector3Normalize(vLook) * m_TransformDesc.SpeedPerSec * TimeDelta;

	_bool isMove = { true };

	if (nullptr != pNavigation)
	{
		isMove = pNavigation->is_Move(vPosition);
	}

	if (true == isMove)
		Set_State(STATE_POSITION, vPosition);
}

void CTransform::Go_Left(_double TimeDelta, CNavigation* pNavigation)
{
	_vector		vPosition = Get_State(STATE_POSITION);
	_vector		vRight = Get_State(STATE_RIGHT);

	vPosition -= XMVector3Normalize(vRight) * m_TransformDesc.SpeedPerSec * TimeDelta;

	_bool isMove = { true };

	if (nullptr != pNavigation)
		isMove = pNavigation->is_Move(vPosition);

	if (true == isMove)
		Set_State(STATE_POSITION, vPosition);
}

void CTransform::Go_Right(_double TimeDelta, CNavigation* pNavigation)
{
	_vector		vPosition = Get_State(STATE_POSITION);
	_vector		vRight = Get_State(STATE_RIGHT);

	vPosition += XMVector3Normalize(vRight) * m_TransformDesc.SpeedPerSec * TimeDelta;

	_bool isMove = { true };

	if (nullptr != pNavigation)
		isMove = pNavigation->is_Move(vPosition);

	if (true == isMove)
		Set_State(STATE_POSITION, vPosition);
}

void CTransform::Go_Direction(_double TimeDelta, DIRECTION eDir, _float fLength)
{
	m_eCurDirection = eDir;
	/*if (m_eCurDirection != DIR_N && m_eCurDirection != DIR_S && m_eCurDirection != DIR_W && m_eCurDirection != DIR_E)
	{
		fLength *= 0.707;
	}*/

	CNavigation* pNavigation = dynamic_cast<CNavigation*>(m_pOwner->Get_Component(L"Com_Navigation"));
	_vector		vPosition = Get_State(STATE_POSITION);
	_vector		vNextPosition = vPosition;
	_vector		vDir = DirectionVector(eDir) * fLength * TimeDelta;
	_bool		isMove = true; // 기본적으로 플레이어는 이동 상태로 가정합니다.

	vNextPosition += vDir;
	if (pNavigation != nullptr)
	{
		if (true == (isMove = pNavigation->is_Move(vNextPosition))) // NonSliding
		{
			vPosition = vNextPosition; // 위치를 업데이트합니다.
			//vPosition -= XMVector3Normalize(XMLoadFloat3(&vContactNormal)) * 0.13f; // 벽에 갇히지 않게 밀어냄
		}
		else
		{
			while (false == isMove)
			{
				vNextPosition = vPosition;
				vDir *= 0.6f;
				_float3 vContactNormal = pNavigation->ContactNormal(); // 충돌 법선을 가져옵니다.
				_vector vSlidingVector = pNavigation->GetSlidingVector(vDir, XMLoadFloat3(&vContactNormal));
				vNextPosition += vSlidingVector * m_TransformDesc.SpeedPerSec * TimeDelta;

				isMove = pNavigation->is_Move(vNextPosition);
			}

			vPosition = vNextPosition; // 위치를 업데이트합니다.
		}
	}
	else
		vPosition = vNextPosition;

	// 이동 가능한 상태라면, 새로 계산한 위치를 설정합니다.
	if (true == isMove)
	{
		Set_State(STATE_POSITION, vPosition);
	}
}



void CTransform::Go_Direction(_double TimeDelta, DIRECTION eDir)
{
	Go_Direction(TimeDelta, eDir, m_TransformDesc.SpeedPerSec);
}

// 내부적으로 노말라이즈 안함.
void CTransform::Go_Direction(_double TimeDelta, _fvector _vDir, _float fLength)
{
	CNavigation* pNavigation = dynamic_cast<CNavigation*>(m_pOwner->Get_Component(L"Com_Navigation"));
	_vector		vPosition = Get_State(STATE_POSITION);
	_vector		vNextPosition = vPosition;
	_vector		vDir = XMVector3Normalize(_vDir); 
	_bool		isMove = true; // 기본적으로 플레이어는 이동 상태로 가정합니다.

	vNextPosition += vDir * fLength * TimeDelta;
	if (pNavigation != nullptr)
	{
		if (true == (isMove = pNavigation->is_Move(vNextPosition))) // NonSliding
		{
			vPosition = vNextPosition; // 위치를 업데이트합니다.
			//vPosition -= XMVector3Normalize(XMLoadFloat3(&vContactNormal)) * 0.13f; // 벽에 갇히지 않게 밀어냄
		}
		else
		{
			while (false == isMove)
			{
				vNextPosition = vPosition;
				vDir *= 0.6f;
				_float3 vContactNormal = pNavigation->ContactNormal(); // 충돌 법선을 가져옵니다.
				_vector vSlidingVector = pNavigation->GetSlidingVector(vDir, XMLoadFloat3(&vContactNormal));
				vNextPosition += vDir * fLength * TimeDelta;

				isMove = pNavigation->is_Move(vNextPosition);
			}

			vPosition = vNextPosition; // 위치를 업데이트합니다.
		}
	}
	else
		vPosition = vNextPosition;

	// 이동 가능한 상태라면, 새로 계산한 위치를 설정합니다.
	if (true == isMove)
	{
		Set_State(STATE_POSITION, vPosition);
	}
}

void CTransform::Jump(_float TimeElapse, _float fPower)
{
	m_WorldMatrix._42 = fPower * TimeElapse - (9.8f * TimeElapse * TimeElapse) * 0.5f;
	m_WorldMatrix._42 = max(m_WorldMatrix._42, 0.f);
}

void CTransform::Chase_Lerp(_fvector vTargetPosition, _double TimeDelta, _float fMinDistance)
{
	//현재 내 위치
	_vector        vPosition = Get_State(STATE_POSITION);
	//거리
	_vector        vDir = vTargetPosition - vPosition;
	if (fMinDistance < XMVectorGetX(XMVector3Length(vDir)))
		vPosition = XMVectorLerp(vPosition, vTargetPosition, m_TransformDesc.SpeedPerSec * TimeDelta);

	Set_State(STATE_POSITION, vPosition);
}

void CTransform::Chase(_fvector vTargetPosition, _double TimeDelta, _float fMinDistance)
{
	_vector		vPosition = Get_State(STATE_POSITION);

	_vector		vDir = vTargetPosition - vPosition;

	if (fMinDistance < XMVectorGetX(XMVector3Length(vDir)))
		vPosition += XMVector3Normalize(vDir) * m_TransformDesc.SpeedPerSec * TimeDelta;

	Set_State(STATE_POSITION, vPosition);
}

void CTransform::LookAt(_fvector vTargetPosition)
{
	_float3		vScaled = Get_Scaled();

	_vector		vPosition = Get_State(STATE_POSITION);

	_vector		vLook = XMVector3Normalize(vTargetPosition - vPosition) * vScaled.z;
	_vector		vRight = XMVector3Normalize(XMVector3Cross(XMVectorSet(0.f, 1.f, 0.f, 0.f), vLook)) * vScaled.x;
	_vector		vUp = XMVector3Normalize(XMVector3Cross(vLook, vRight)) * vScaled.y;

	Set_State(STATE_RIGHT, vRight);
	Set_State(STATE_UP, vUp);
	Set_State(STATE_LOOK, vLook);
}

/* 항등상태를 기준으로 지정한 각만큼 회전시킨다. */
void CTransform::Rotation(_fvector vAxis, _float fRadian)
{
	_float3		vScaled = Get_Scaled();

	_vector		vRight = XMVectorSet(1.f, 0.f, 0.f, 0.f) * vScaled.x;
	_vector		vUp = XMVectorSet(0.f, 1.f, 0.f, 0.f) * vScaled.y;
	_vector		vLook = XMVectorSet(0.f, 0.f, 1.f, 0.f) * vScaled.z;

	_matrix		RotationMatrix = XMMatrixRotationAxis(vAxis, fRadian);

	/*XMVector3TransformCoord();*/
	/*XMVector4Transform();*/
	Set_State(STATE_RIGHT, XMVector3TransformNormal(vRight, RotationMatrix));
	Set_State(STATE_UP, XMVector3TransformNormal(vUp, RotationMatrix));
	Set_State(STATE_LOOK, XMVector3TransformNormal(vLook, RotationMatrix));
}

void CTransform::RotationBack()
{
	m_WorldMatrix._11 *= -1.f;
	m_WorldMatrix._12 *= -1.f;
	m_WorldMatrix._13 *= -1.f;

	m_WorldMatrix._31 *= -1.f;
	m_WorldMatrix._32 *= -1.f;
	m_WorldMatrix._33 *= -1.f;
}

void CTransform::Rotation(_fmatrix RotationMatrixX, _fmatrix RotationMatrixY, _fmatrix RotationMatrixZ)
{
	_float3 vScale = Get_Scaled();
	_float3 vPos;
	XMStoreFloat3(&vPos, Get_State(STATE_POSITION));

	_matrix matScale = XMMatrixScaling(vScale.x, vScale.y, vScale.z);
	_matrix matRot = RotationMatrixX * RotationMatrixY * RotationMatrixZ;
	_matrix matPos = XMMatrixTranslation(vPos.x, vPos.y, vPos.z);

	XMStoreFloat4x4(&m_WorldMatrix, matScale * matRot * matPos);
}

void CTransform::Turn(_fvector vAxis, _double TimeDelta)
{
	_vector		vRight = Get_State(STATE_RIGHT);
	_vector		vUp = Get_State(STATE_UP);
	_vector		vLook = Get_State(STATE_LOOK);

	_matrix		RotationMatrix = XMMatrixRotationAxis(vAxis, m_TransformDesc.RotationPerSec * TimeDelta);

	Set_State(STATE_RIGHT, XMVector3TransformNormal(vRight, RotationMatrix));
	Set_State(STATE_UP, XMVector3TransformNormal(vUp, RotationMatrix));
	Set_State(STATE_LOOK, XMVector3TransformNormal(vLook, RotationMatrix));
}

void CTransform::Turn(_fvector vAxis, _fvector _vTargetVector, _double TimeDelta)
{
	_vector		vLook = Get_State(STATE_LOOK);
	_vector		vTargetVector = _vTargetVector;
	_float fCross = XMVectorGetY(XMVector3Cross(vLook, vTargetVector));
	_float fDegree = fabs(DegreeBetweenVectors(vTargetVector, vLook));

	if (DegreeBetweenVectors(_vTargetVector, Get_State(STATE_LOOK)) < 3.f)
		return;

	// 0도 = 360도이기 때문에 360도에서도 더 이상 회전 안시키려고 넣은 코드.
	if (fDegree > 359.9f)
		fDegree = 0.f;

	if (fDegree < m_TransformDesc.RotationPerSec + 1.f)
	{
		Rotation(vAxis, RadianBetweenVectors(vTargetVector, WorldAxisZ()));
		return;
	}

	if (fCross < 0.f)
		TimeDelta *= -1.0;
	Turn(vAxis, TimeDelta);
}	

void CTransform::Scaled(const _float3& vScale)
{
	Set_State(STATE_RIGHT,
		XMVector3Normalize(Get_State(STATE_RIGHT)) * vScale.x);
	Set_State(STATE_UP,
		XMVector3Normalize(Get_State(STATE_UP)) * vScale.y);
	Set_State(STATE_LOOK,
		XMVector3Normalize(Get_State(STATE_LOOK)) * vScale.z);

}

_float3 CTransform::DeltaPosition()
{
	return _float3(m_WorldMatrix.m[STATE_POSITION][0] - m_PrevWorldMatrix.m[STATE_POSITION][0]
	, m_WorldMatrix.m[STATE_POSITION][1] - m_PrevWorldMatrix.m[STATE_POSITION][1]
	, m_WorldMatrix.m[STATE_POSITION][2] - m_PrevWorldMatrix.m[STATE_POSITION][2]);
}

_bool CTransform::isFront(_fvector vPosition, _float fDistance, _float fDegree)
{
	_vector vMyPosition = Get_State(CTransform::STATE_POSITION);
	_vector vDir = vPosition - vMyPosition;
	_vector vLook = Get_State(CTransform::STATE_LOOK);

	if (XMVectorGetX(XMVector3Length(vDir)) > fDistance)
		return false;

	vDir = XMVector3Normalize(vDir);
	vLook = XMVector3Normalize(vLook);

	float fDot = XMVectorGetX(XMVector3Dot(vDir, vLook));
	float fRadian = acos(fDot);
	fRadian *= 2.f;

	return fRadian < XMConvertToRadians(fDegree);;
}



_vector CTransform::DirectionVector(DIRECTION eDir)
{
	switch (eDir)
	{
	case DIR_N:
		return XMVectorSet(0.f, 0.f, 1.f, 0.f);
	case DIR_NE:
		return XMVector3Normalize(XMVectorSet(1.f, 0.f, 1.f, 0.f));
	case DIR_E:
		return XMVectorSet(1.f, 0.f, 0.f, 0.f);
	case DIR_SE:
		return XMVector3Normalize(XMVectorSet(1.f, 0.f, -1.f, 0.f));
	case DIR_S:
		return XMVectorSet(0.f, 0.f, -1.f, 0.f);
	case DIR_SW:
		return XMVector3Normalize(XMVectorSet(-1.f, 0.f, -1.f, 0.f));
	case DIR_W:
		return XMVectorSet(-1.f, 0.f, 0.f, 0.f);
	case DIR_NW:
		return XMVector3Normalize(XMVectorSet(-1.f, 0.f, 1.f, 0.f));
	default:
		break;
	}

}

CTransform* CTransform::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CTransform* pInstance = new CTransform(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created CTransform");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CComponent* CTransform::Clone(void* pArg)
{
	CTransform* pInstance = new CTransform(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned CTransform");
		Safe_Release(pInstance);
	}
	return pInstance;
}


void CTransform::Free()
{
	__super::Free();


}