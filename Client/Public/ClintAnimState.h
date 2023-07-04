#pragma once

#include "Client_Defines.h"
#include "AnimState.h"

BEGIN(Client)
class ClintAnimState abstract : public AnimState
{
protected:
	ClintAnimState(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~ClintAnimState() = default;

public:
	virtual HRESULT Initialize(Clint* pOwner);
	virtual void Tick(_double TimeDelta) override;
	virtual void Late_Tick(_double TimeDelta) override;

	void Set_Owner(Clint* pOwner) {
		m_pOwner = pOwner;
	}
protected:
	class Clint* m_pOwner = { nullptr };
public:
	virtual void Free() override;
};

END
