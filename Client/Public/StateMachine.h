#pragma once

/*
1. 상태 헤더파일도 포함하기.
2. 원하는 상태(스니펫 활용)를 정의하고 Add_State로 StateMachine의 umap에 넣는다.
3. 원하는 곳에 TransitionTo를 정의한다.
*/

#include "Component.h"
#include "ClientInstance.h"
#include "IStateMachine.h"
#include "Collider.h"
#include "GameInstance.h"

BEGIN(Client)

template <typename OWNER, typename ANIM_ENUM>
class StateContext;

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
	StateMachine() = default;
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
	void TransitionTo(const _tchar* pTag) {
		m_pStateContext->TransitionTo(pTag);
	}

	virtual const _tchar* GetTag() = 0;

	virtual void OnCollision(CCollider::COLLISION_INFO tCollisionInfo, _double TimeDelta) {};

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


BEGIN(Client)
template <typename OWNER, typename ANIM_ENUM>
class StateContext final : public CComponent
{
public:
	typedef class StateMachine<OWNER, ANIM_ENUM> State;
public:
	typedef struct tagStateContextDesc : public tagComponentDesc
	{
		tagStateContextDesc() : tagComponentDesc() {};
	}STATE_CONTEXT_DESC;

public:
	StateContext() = default;
	explicit StateContext(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) : CComponent(pDevice, pContext) {}
	explicit StateContext(const StateContext& rhs) : CComponent(rhs)
	{
		for (auto& Pair : rhs.m_pStates)
		{
			m_pStates.emplace(Pair.first, Pair.second->Clone(nullptr));
		}
	}
	virtual ~StateContext() override = default;

public:
	void Tick(_double TimeDelta) {
		if (nullptr != m_pCurState)
			m_pCurState->OnStateTick(TimeDelta);
	}

public:
	void Add_State(const _tchar* pTag, State* pState) {
		if (nullptr != Find_State(pTag))
			assert(false);

		m_pStates.emplace(pTag, pState);
	}

	void TransitionTo(const _tchar* pTag) {
		if (nullptr != m_pCurState)
			m_pCurState->OnStateExit();
		m_pCurState = Find_State(pTag);
		NULL_CHECK_RETURN(m_pCurState);
		m_pCurState->OnStateEnter();
	}

	State* Find_State(const _tchar* pTag) {
		auto iter = find_if(m_pStates.begin(), m_pStates.end(), CTag_Finder(pTag));

		if (iter == m_pStates.end())
			return nullptr;

		return iter->second;
	}

	void OnCollision(CCollider::COLLISION_INFO tCollisionInfo, _double TimeDelta)
	{
		m_pCurState->OnCollision(tCollisionInfo, TimeDelta);
	}

	const _tchar* GetCurState() {
		return m_pCurState->GetTag();
	}

public:
	virtual HRESULT Initialize_Prototype() {
		__super::Initialize_Prototype();

		return S_OK;
	}

	virtual HRESULT Initialize(void* pArg) override {
		if (nullptr == pArg)
			return S_OK;

		FAILED_CHECK_RETURN(__super::Initialize(pArg), E_FAIL);

		typename State::STATE_MACHINE_DESC tStateMachineDesc;
		tStateMachineDesc.pOwner = static_cast<OWNER*>(m_pOwner);
		tStateMachineDesc.pStateContext = this;
		for (auto& Pair : m_pStates)
		{
			Pair.second->Initialize(&tStateMachineDesc);
		}

		return S_OK;
	}

private:
	unordered_map<const _tchar*, State*> m_pStates;
	State* m_pCurState = { nullptr };

public:
	static StateContext* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) {
		StateContext<OWNER, ANIM_ENUM>* pInstance = new StateContext<OWNER, ANIM_ENUM>(pDevice, pContext);

		if (FAILED(pInstance->Initialize_Prototype()))
		{
			MSG_BOX("Failed to Created StateContext<OWNER, ANIM_ENUM>");
			Safe_Release(pInstance);
		}
		return pInstance;
	}

	virtual CComponent* Clone(void* pArg) override {
		StateContext<OWNER, ANIM_ENUM>* pInstance = new StateContext<OWNER, ANIM_ENUM>(*this);

		if (FAILED(pInstance->Initialize(pArg)))
		{
			MSG_BOX("Failed to Cloned StateContext<OWNER, ANIM_ENUM>");
			Safe_Release(pInstance);
		}
		return pInstance;
	}

	virtual void Free(void) {
		__super::Free();

		for (auto& Pair : m_pStates)
		{
			Safe_Release(Pair.second);
		}
		m_pStates.clear();
	}
};

END