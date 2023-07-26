
#pragma once
#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL TimeConuter final : public CComponent
{
public:
	typedef struct tagTimeConuterDesc : public tagComponentDesc
	{
		tagTimeConuterDesc() : tagComponentDesc() {};
	}TIME_COUNTER_DESC;
private:
	explicit TimeConuter(ID3D11Device * pDevice, ID3D11DeviceContext * pContext);
	explicit TimeConuter(const TimeConuter& rhs);
	virtual ~TimeConuter() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg) override;
	void Tick(_double TimeDelta);

public:
	void Reset() { m_TimeAcc = 0.0; }
	void Enable() { m_bEnable = true; }
	void DisEnable() { m_bEnable = false; }
	_bool isEuqalWith(_double Time, _double epsilon = std::numeric_limits<_double>::epsilon());
	_bool isGreaterThan(_double Time);
	_bool isLessThan(_double Time);
	_bool EveryPer(_double Time, _double epsilon = std::numeric_limits<_double>::epsilon());

private:
	_double m_TimeAcc = { 0.0 };
	_bool	m_bEnable = { false };

public:
	// If this component is the VIBuffer Com or the Shader Com, Delete ProtoTag().
	static const _tchar* ProtoTag() { return L"Prototype_Component_TimeConuter"; }
	static TimeConuter* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END
