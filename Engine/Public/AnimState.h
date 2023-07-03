
#pragma once

#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL AnimState abstract : public CBase
{
protected:
	AnimState(ID3D11Device * pDevice, ID3D11DeviceContext * pContext);
	virtual ~AnimState() = default;

public:
	virtual void Tick(_double TimeDelta) = 0;
	virtual void Late_Tick(_double TimeDelta) = 0;

protected:
	ID3D11Device* m_pDevice = { nullptr };
	ID3D11DeviceContext* m_pContext = { nullptr };

public:
	virtual void Free() override;
};

END
