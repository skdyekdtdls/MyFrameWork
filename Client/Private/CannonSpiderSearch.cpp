#include "..\Public\CannonSpiderSearch.h"
#include "GameInstance.h"
#include "CannonSpider.h"
#include "Bullet.h"
CannonSpiderSearch::CannonSpiderSearch(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: StateMachine<CannonSpider, CANNON_SPIDER_ANIM>(pDevice, pContext)
{
}

CannonSpiderSearch::CannonSpiderSearch(const CannonSpiderSearch& rhs)
	: StateMachine<CannonSpider, CANNON_SPIDER_ANIM>(rhs)
	, m_eCurLogic(LOGIC_END)
{
}

void CannonSpiderSearch::OnStateEnter()
{
	__super::OnStateEnter();

	RandomLogic();
	SetAnimIndexByCurLogic();
	
}

void CannonSpiderSearch::OnStateTick(_double TimeDelta)
{
	__super::OnStateTick(TimeDelta);

	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);
	CModel* pModel = static_cast<CModel*>(m_pOwner->Get_Component(L"Com_Model"));
	CTransform* pTransform = static_cast<CTransform*>(m_pOwner->Get_Component(L"Com_Transform"));
	
	if (m_bStartTime)
	{
		m_TimeAcc += TimeDelta;
	}
	
	if (m_bStartTime && m_TimeAcc >= 3.f)
	{
		m_bStartTime = false;
		m_TimeAcc = 0.0;
	}
	if (pModel->IsAnimationFinished() && false == m_bStartTime)
	{
		m_bStartTime = { true };
		RandomLogic();
		SetAnimIndexByCurLogic();
		//SetCurLook(pTransform);
		CurLogicUpdate(TimeDelta);
	}

	Safe_Release(pGameInstance);
}

void CannonSpiderSearch::OnStateExit()
{
	__super::OnStateExit();
}

void CannonSpiderSearch::OnCollision(CCollider::COLLISION_INFO tCollisionInfo, _double TimeDelta)
{

}

void CannonSpiderSearch::RandomLogic()
{
	LOGIC ePrevLogic = m_eCurLogic;

	while (m_eCurLogic == ePrevLogic)
	{
		m_eCurLogic = static_cast<LOGIC>(RandomIntFrom_A_To_B(0, LOGIC_END - 1));
	}
}

void CannonSpiderSearch::SetAnimIndexByCurLogic()
{
	switch (m_eCurLogic)
	{
	case Client::CannonSpiderSearch::LOGIC1:
		SetAnimIndex(CANNON_SPIDER_TURN180_L, LOWER);
		cout << "CANNON_SPIDER_TURN180_L" << endl;
		break;
	case Client::CannonSpiderSearch::LOGIC2:
		SetAnimIndex(CANNON_SPIDER_TURN90_L, LOWER);
		cout << "CANNON_SPIDER_TURN90_L" << endl;
		break;
	case Client::CannonSpiderSearch::LOGIC3:
		SetAnimIndex(CANNON_SPIDER_TURN90_R, LOWER);
		cout << "CANNON_SPIDER_TURN90_R" << endl;
		break;
	}
}

void CannonSpiderSearch::SetCurLook(CTransform* pTransform)
{
	switch (m_eCurLogic)
	{
	case Client::CannonSpiderSearch::LOGIC1:
		pTransform->Rotation(WorldAxisY(), XMConvertToRadians(180.f));
		break;
	case Client::CannonSpiderSearch::LOGIC2:
		pTransform->Rotation(WorldAxisY(), XMConvertToRadians(-90.f));
		break;
	case Client::CannonSpiderSearch::LOGIC3:
		pTransform->Rotation(WorldAxisY(), XMConvertToRadians(90.f));
		break;
	}
}

void CannonSpiderSearch::CurLogicUpdate(const _double& TimeDelta)
{
	switch (m_eCurLogic)
	{
	case LOGIC1:
		Logic1(TimeDelta);
		break;
	case LOGIC2:
		Logic2(TimeDelta);
		break;
	case LOGIC3:
		Logic3(TimeDelta);
		break;
	}
}

void CannonSpiderSearch::Logic1(const _double& TimeDelta)
{
}

void CannonSpiderSearch::Logic2(const _double& TimeDelta)
{
}

void CannonSpiderSearch::Logic3(const _double& TimeDelta)
{
}

StateMachine<CannonSpider, CANNON_SPIDER_ANIM>* CannonSpiderSearch::Clone(void* pArg)
{
	__super::Initialize(pArg);
	return new CannonSpiderSearch(*this);
}

CannonSpiderSearch* CannonSpiderSearch::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	return new CannonSpiderSearch(pDevice, pContext);
}

void CannonSpiderSearch::Free()
{
	__super::Free();
}