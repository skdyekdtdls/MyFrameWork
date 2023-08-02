
#pragma once
#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CTransform2D final : public CComponent
{
public:
	typedef struct tagCTransform2DDesc : public tagComponentDesc
	{
		tagCTransform2DDesc() : tagComponentDesc() {};
		tagCTransform2DDesc(_double _SpeedPerSec, _double _RoationPerSec)
			: SpeedPerSec{ _SpeedPerSec }
			, RotationPerSec{ _RoationPerSec }
		{

		};

		_double		SpeedPerSec = { 10.0 };
		_double		RotationPerSec = { 10.0 };
	}CTRANSFORM2D_DESC;

private:
	explicit CTransform2D(ID3D11Device * pDevice, ID3D11DeviceContext * pContext);
	explicit CTransform2D(const CTransform2D& rhs);
	virtual ~CTransform2D() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg) override;

	void Go_Up(const _double& TimeDelta);
	void Go_Down(const _double& TimeDelta);
	void Go_Left(const _double& TimeDelta);
	void Go_Right(const _double& TimeDelta);

	void Jump(_float TimeElapse, _float fPower, _float fGravity = 18.f);
	void Chase(const _double& TimeDelta, _fvector vTargetPosition, _float fMinDistance = 0.1f);
	void Rotation(_float fDegree);
	void TurnCCW(const _double& TimeDelta);
	void TurnCW(const _double& TimeDelta);

	void Scaled(const _float2& vScale);

	void SetMoveSpeed(_float fMoveSpeed);
	void SetRotationSpeed(_float RotationSpeed);
	
	_float Get_AulerDegree();
	_float2 GetScale();
	_float4x4 Get_WorldFloat4x4() { return m_WorldMatrix; }

public:
	_vector GetPosition();
	void SetPosition(_fvector vPosition);
	void SetDepth(_float fDepth);
private:
	CTRANSFORM2D_DESC			m_TransformDesc;
	_float4x4					m_WorldMatrix;

public:
	// If this component is the VIBuffer Com or the Shader Com, Delete ProtoTag().
	static const _tchar* ProtoTag() { return L"Prototype_Component_CTransform2D"; }
	static CTransform2D* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END
