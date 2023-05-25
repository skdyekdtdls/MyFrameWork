#pragma once

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CTransform final : public CComponent
{
public:
	enum STATE { STATE_RIGHT, STATE_UP, STATE_LOOK, STATE_POSITION, STATE_END };
public:
	typedef struct tagTransformDesc
	{
		tagTransformDesc(_double _SpeedPerSecond, _double _RotationPerSecond)
			: SpeedPerSecond{ _SpeedPerSecond }
			, RotationPerSecond{ _RotationPerSecond }
		{}
		_double SpeedPerSecond = { 0.0 };
		_double RotationPerSecond = { 0.0 };
	}TRANSFORMDESC;

private:
	CTransform(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CTransform(const CTransform& rhs);
	virtual ~CTransform() = default;

public:
	_vector Get_State(STATE _eState)
	{
		return XMLoadFloat4x4(&m_WorldMatrix).r[_eState];
	}
	void Set_State(STATE _eState, _fvector _vState)
	{
		_float4 vState;
		XMStoreFloat4(&vState, _vState);
		memcpy(&m_WorldMatrix.m[_eState][0], &vState, sizeof vState);
	}
public:
	HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg) override;

public:
	void Go_Straight(_double TimeDelta);

private:
	TRANSFORMDESC m_TransformDesc;

private:
	_float4x4 m_WorldMatrix;

public:
	static CTransform* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free(void) override;
};
END