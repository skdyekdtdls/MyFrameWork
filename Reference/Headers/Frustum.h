#pragma once

#include "Base.h"

BEGIN(Engine)

class Frustum final : public CBase
{
	DECLARE_SINGLETON(Frustum)
private:
	Frustum();
	virtual ~Frustum() = default;

public:
	HRESULT Initialize();
	void Tick();
	_bool isIn_WorldSpace(_fvector vWorldPos, _float fRange);

private:
	_float3			m_vOriginal_Points[8];
	_float4			m_vWorldPlanes[6];

public:
	virtual void Free() override;
};

END