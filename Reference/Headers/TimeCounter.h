
#pragma once
#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL TimeCounter final : public CComponent
{
private:
	enum COUNTER { NORMAL, EVERY, COUNTER_END};
public:
	typedef struct tagTimeCounterDesc : public tagComponentDesc
	{
		tagTimeCounterDesc() : tagComponentDesc() {};
	}TIME_COUNTER_DESC;
private:
	explicit TimeCounter(ID3D11Device * pDevice, ID3D11DeviceContext * pContext);
	explicit TimeCounter(const TimeCounter& rhs);
	virtual ~TimeCounter() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg) override;
	void Tick(_double TimeDelta);

public:
	void Reset();
	void Enable() { m_bEnable = true; }
	void DisEnable() { m_bEnable = false; }
	_bool isEuqalWith(_double Time);
	_bool isGreaterThan(_double Time);
	_bool isLessThan(_double Time);
	_bool EveryPer(_double Time);

private:
	_double m_TimeAcc[COUNTER_END];
	_bool	m_bEnable = { false };
	_bool   m_bEqualResult = { false };
public:
	// If this component is the VIBuffer Com or the Shader Com, Delete ProtoTag().
	static const _tchar* ProtoTag() { return L"Prototype_Component_TimeCounter"; }
	static TimeCounter* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END