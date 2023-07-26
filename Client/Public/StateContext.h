
/*
0. ANIM_ENUM에 해당하는 열거형을 Cline_Enum.h에 정의한다.
1. StateMachine<OWNER, ANIM_ENUM>를 로더에 Create한다(상태 헤더파일도 포함하기).
2. 원하는 상태(스니펫 활용)를 정의하고 Add_State로 StateMachine의 umap에 넣는다.
3. 게임오브젝트에 typedef StateContext<OWNER, ANIM_ENUM> "X + State"로 정의한다.; 
4. 컴포넌트 추가 과정을 거친다.
	4-1. 변수 선언
	4-2. Add_Component에 추가
	4-3. Tick함수 호출
	4-4. Safe_Release
5. Initialize에서 컴포넌트의 TransitonTo()를 호출한다.
*/

#pragma once
#include "Component.h"
#include "StateContext.h"
#include "StateMachine.h"
#include "GameInstance.h"
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
	void Tick(_double TimeDelta){
		if (nullptr != m_pCurState)
			m_pCurState->OnStateTick(TimeDelta);
	}

public:
	void Add_State(const _tchar* pTag, State* pState){
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

	virtual void Free(void ) {
		__super::Free();

		for (auto& Pair : m_pStates)
		{
			Safe_Release(Pair.second);
		}
		m_pStates.clear();
	}
};

END