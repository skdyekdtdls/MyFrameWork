#pragma once

#include "Component.h"
#include "ISerializable.h"
/* 객체의 월드 변환 상태를 가진다.(월드행렬을 보관한다.) */
/* 월드에서의 변환 기능을 가진다.(Go_Straight(), Go_Backword(), Turn() )*/

BEGIN(Engine)

class ENGINE_DLL CTransform final : public CComponent, public ISerializable
{
public:
	enum STATE { STATE_RIGHT, STATE_UP, STATE_LOOK, STATE_POSITION, STATE_END };
	enum DIRECTION { DIR_N, DIR_NE, DIR_E, DIR_SE, DIR_S, DIR_SW, DIR_W, DIR_NW, DIR_END };

public:
	typedef struct tagCTransformDesc : public tagComponentDesc
	{
		tagCTransformDesc() : tagComponentDesc() {};
		tagCTransformDesc(_double _SpeedPerSec, _double _RoationPerSec)
		: SpeedPerSec { _SpeedPerSec }
		, RotationPerSec { _RoationPerSec }
		{

		}

		_double		SpeedPerSec = { 0.0 };
		_double		RotationPerSec = { 0.0 };
	}CTRANSFORM_DESC;

private:
	CTransform(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CTransform(const CTransform& rhs);
	CTransform& operator=(const CTransform& other) = delete;
	virtual ~CTransform() = default;

public:
	virtual void Save(HANDLE hFile, DWORD& dwByte) override;
	virtual void Load(HANDLE hFile, DWORD& dwByte, _uint iLevelIndex) override;

public:
	_float2 GetScreenPos(_uint iWinSizeX, _uint iWinSizeY);

	DIRECTION GetCurDirection() {
		return m_eCurDirection;
	}

	_vector Get_State(STATE _eState) {
		return XMLoadFloat4x4(&m_WorldMatrix).r[_eState];
	}
	_float3 Get_Scaled();
	_float3 Get_AulerDegree();

	_float4x4* Get_WorldFloat4x4Ptr() {
		return &m_WorldMatrix;
	}

	_float4x4 Get_WorldFloat4x4() {
		return m_WorldMatrix;
	}

	_matrix Get_WorldMatrix() {
		return XMLoadFloat4x4(&m_WorldMatrix);
	}

	void Set_State(STATE _eState, _fvector _vState);

	_matrix Get_WorldMatrix_Inverse() const {
		return XMMatrixInverse(nullptr, XMLoadFloat4x4(&m_WorldMatrix));
	}

	void Set_Desc(const CTRANSFORM_DESC& TransformDesc) {
		m_TransformDesc = TransformDesc;
	}

	void Set_WorldMatrix(_fmatrix matWorldMatrix) {
		memcpy(&m_PrevWorldMatrix, &m_WorldMatrix, sizeof(_float4x4));
		XMStoreFloat4x4(&m_WorldMatrix, matWorldMatrix);
	}

	void Set_Speed(float fSpeed)
	{
		m_TransformDesc.SpeedPerSec = fSpeed;
	}

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
public:
	_vector DirectionVector(DIRECTION eDir);

public:
	void Chase_Lerp(_fvector vTargetPosition, _double TimeDelta, _float fMinDistance);

	void Go_Straight(_double TimeDelta, class CNavigation* pNavigation = nullptr);
	void Go_Backward(_double TimeDelta, class CNavigation* pNavigation = nullptr);
	void Go_Left(_double TimeDelta, class CNavigation* pNavigation = nullptr);
	void Go_Right(_double TimeDelta, class CNavigation* pNavigation = nullptr);

	void Go_Direction(_double TimeDelta, DIRECTION eDir, _float fLength);
	void Go_Direction(_double TimeDelta, DIRECTION eDir);
	void Go_Direction(_double TimeDelta, _fvector vDir, _float fLength);
	void Jump(_float TimeElapse, _float fPower);
	void Chase(_fvector vTargetPosition, _double TimeDelta, _float fMinDistance = 0.1f);
	void LookAt(_fvector vTargetPosition);

	// 항등 상태를 기준으로 지정한 각만큼 회전시킨다.
	void Rotation(_fvector vAxis, _float fRadian);
	void RotationBack();
	void Rotation(_fmatrix RotationMatrixX, _fmatrix RotationMatrixY, _fmatrix RotationMatrixZ);
	void Turn(_fvector vAxis, _double TimeDelta);
	void Turn(_fvector vAxis, _fvector vTargetVector, _double TimeDelta);
	void Scaled(const _float3 & vScale);

	// 이전 프레임과 현재 프레임 사이의 위치 변화율을 반환합니다.
	_float3 DeltaPosition();

	_bool isFront(_fvector vPosition, _float fDistance = FLT_MAX, _float fDegree = 180.f);

private:
	CTRANSFORM_DESC			m_TransformDesc;
	DIRECTION				m_eCurDirection;
private:
	_float4x4				m_PrevWorldMatrix;
	_float4x4				m_WorldMatrix;

public:
	static const _tchar* ProtoTag() { return L"Prototype_Component_Transform"; }
	static CTransform* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END