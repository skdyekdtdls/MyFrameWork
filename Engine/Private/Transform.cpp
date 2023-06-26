#include "..\Public\Transform.h"
#include "Navigation.h"



CTransform::CTransform(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CComponent(pDevice, pContext)
	, m_TransformDesc(0.0, 0.0)
{
}

CTransform::CTransform(const CTransform& rhs)
	: CComponent(rhs)
	, m_TransformDesc(rhs.m_TransformDesc)
	, m_WorldMatrix(rhs.m_WorldMatrix)
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

	memcpy(&m_WorldMatrix.m[_eState][0], &vState, sizeof vState);
}

HRESULT CTransform::Initialize_Prototype()
{
	XMStoreFloat4x4(&m_WorldMatrix, XMMatrixIdentity());

	return S_OK;
}

HRESULT CTransform::Initialize(void* pArg)
{
	if (nullptr != pArg)
		memmove(&m_TransformDesc, pArg, sizeof m_TransformDesc);

	return S_OK;
}

void CTransform::Go_Straight(_double TimeDelta, CNavigation* pNavigation)
{
	_vector		vPosition = Get_State(STATE_POSITION);
	_vector		vLook = Get_State(STATE_LOOK);

	_bool isMove = true; // 기본적으로 플레이어는 이동 상태로 가정합니다.

	if (pNavigation != nullptr)
	{
		isMove = pNavigation->is_Move(vPosition); // 네비게이션으로부터 이동 가능 여부를 확인합니다.

		if (false == isMove)
		{
			_float3 vContactNormal = pNavigation->ContactNormal(); // 충돌 법선을 가져옵니다.
			// 법선이 유효하다면 슬라이딩 벡터를 계산합니다.
			if (!Float3Equal(_float3(), vContactNormal))
			{
				_vector N = XMVector3Normalize(XMLoadFloat3(&vContactNormal)); // 충돌직선의 법선 벡터
				_vector P = XMVector3Normalize(vLook); // 플레이어 이동벡터
				_vector S = P - N * ((XMVector3Dot(P, N))); // 슬라이딩 벡터를 계산합니다.
				
				_vector NextPos = vPosition + S * m_TransformDesc.SpeedPerSec * TimeDelta; // 위치를 업데이트합니다.
				NextPos -= N * 0.13f * TimeDelta; // 벽에 갇히지 않게 밀어냄

				// 슬라이딩 위치가 현재 삼각형을 벗어나면 새로운 검사를 진행한다.
				if (false == pNavigation->is_Move(NextPos))
				{
					vContactNormal = pNavigation->ContactNormal();

					if (!Float3Equal(_float3(), vContactNormal))
					{
						_vector N = XMVector3Normalize(XMLoadFloat3(&vContactNormal)); // 충돌직선의 법선 벡터
						_vector P = XMVector3Normalize(vLook); // 플레이어 이동벡터
						_vector S = P - N * ((XMVector3Dot(P, N))); // 슬라이딩 벡터를 계산합니다.

						vPosition += S * m_TransformDesc.SpeedPerSec * TimeDelta; // 위치를 업데이트합니다.
						vPosition -= N * 0.13f * TimeDelta; // 벽에 갇히지 않게 밀어냄
					}
				}
				else
					vPosition = NextPos; // 위치를 업데이트합니다.
			}
			else
			{
				isMove = false; // 법선이 유효하지 않다면 이동을 멈춥니다.
			}
		}
		else
		{
			// 이동 가능한 경우, 이동 벡터를 계산하여 위치를 업데이트합니다.
			vPosition += XMVector3Normalize(vLook) * m_TransformDesc.SpeedPerSec * TimeDelta;
		}
	}
	else
	{
		// 네비게이션이 없는 경우, 이동 벡터를 계산하여 위치를 업데이트합니다.
		vPosition += XMVector3Normalize(vLook) * m_TransformDesc.SpeedPerSec * TimeDelta;
	}

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

	/*XMVector3TransformCoord();*/
	/*XMVector4Transform();*/
	Set_State(STATE_RIGHT, XMVector3TransformNormal(vRight, RotationMatrix));
	Set_State(STATE_UP, XMVector3TransformNormal(vUp, RotationMatrix));
	Set_State(STATE_LOOK, XMVector3TransformNormal(vLook, RotationMatrix));
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