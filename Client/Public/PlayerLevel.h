
#pragma once
#include "Component.h"
#include "Client_Defines.h"
#include "IObserver.h"

BEGIN(Engine)

class ENGINE_DLL PlayerLevel final : public CComponent, public IObserver
{
public:
	typedef struct tagPlayerLevelDesc : public tagComponentDesc
	{
		tagPlayerLevelDesc() : tagComponentDesc() {};
	};
private:
	explicit PlayerLevel(ID3D11Device * pDevice, ID3D11DeviceContext * pContext);
	explicit PlayerLevel(const PlayerLevel& rhs);
	virtual ~PlayerLevel() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg) override;

	void LevelUp();

public:
	virtual void Subscribe(const _tchar* pTag, function<void()> CallBack);
	virtual void UnSubscribe(const _tchar* pTag);
	virtual void UnSubscribeDelay(const _tchar* pTag);

private:
	_uint m_iLevel = { 1 };
	_float m_fExp = { 0.f };

public:
	// If this component is the VIBuffer Com or the Shader Com, Delete ProtoTag().
	static const _tchar* ProtoTag() { return L"Prototype_Component_PlayerLevel"; }
	static PlayerLevel* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END
