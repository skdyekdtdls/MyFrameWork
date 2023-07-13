#pragma once

/*
1. StateMachine<OWNER, ANIM_ENUM>를 로더에 Create한다(상태 헤더파일도 포함하기).
2. 원하는 상태(스니펫 활용)를 정의하고 Add_State로 StateMachine의 umap에 넣는다.
3. 원하는 곳에 TransitionTo를 정의한다.
*/

#include "ClientInstance.h"
#include "IStateMachine.h"
//#include "StateContext.h"

BEGIN(Client)

template<typename OWNER, typename ANIM_ENUM>
class StateMachine abstract : public CBase, public IStateMachine
{
public:
	typedef struct tagStateMachineDesc
	{
		tagStateMachineDesc() {};
		OWNER* pOwner;
		class StateContext<OWNER, ANIM_ENUM>* pStateContext;
	}STATE_MACHINE_DESC;

protected:
	StateMachine() {};
	StateMachine(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
		: m_pDevice(pDevice)
		, m_pContext(pContext)
		, m_isCloned(false)
	{
		Safe_AddRef(m_pDevice);
		Safe_AddRef(m_pContext);
	}
	StateMachine(const StateMachine& rhs)
		: m_pDevice(rhs.m_pDevice)
		, m_pContext(rhs.m_pContext)
		, m_isCloned(true)
	{
		Safe_AddRef(m_pDevice);
		Safe_AddRef(m_pContext);
	}

	virtual ~StateMachine() override = default;

public:
	virtual void Initialize(void* pArg) {
		if (nullptr == pArg)
			return;
		STATE_MACHINE_DESC tStateMachineDesc = *(STATE_MACHINE_DESC*)pArg;
		m_pOwner = tStateMachineDesc.pOwner;
		m_pStateContext = tStateMachineDesc.pStateContext;
	};

	virtual void OnStateEnter() override {};
	virtual void OnStateTick(_double TimeDelta) override {};
	virtual void OnStateExit() override {};

	void SetAnimIndex(ANIM_ENUM eAnim, BODY eBody = LOWER) {
		static_cast<CModel*>(m_pOwner->Get_Component(L"Com_Model"))->Set_AnimByIndex(eAnim, eBody);
	}

protected:
	_bool	m_isCloned;
	OWNER* m_pOwner = { nullptr };
	StateContext<OWNER, ANIM_ENUM>* m_pStateContext;
	ID3D11Device* m_pDevice = { nullptr };
	ID3D11DeviceContext* m_pContext = { nullptr };

public:
	virtual StateMachine<OWNER, ANIM_ENUM>* Clone(void* pArg) = 0;

	virtual void Free(void) {
		Safe_Release(m_pDevice);
		Safe_Release(m_pContext);
	}
};

END