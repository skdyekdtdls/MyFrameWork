#include "..\Public\ClintUltimate01.h"
#include "GameInstance.h"
#include "Clint.h"
#include "ClintUltimate01Bullet.h"
ClintUltimate01::ClintUltimate01(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: StateMachine<Clint, CLINT_ANIM>(pDevice, pContext)
{
}

ClintUltimate01::ClintUltimate01(const ClintUltimate01& rhs)
	: StateMachine<Clint, CLINT_ANIM>(rhs)
	, m_TimeAcc(0.0)
	, m_Duration(6.0)
{
}

void ClintUltimate01::OnStateEnter()
{
	__super::OnStateEnter();

	SetAnimIndex(CLINT_ULTIMATE01, LOWER);
	SetAnimIndex(CLINT_ULTIMATE01, UPPER);

	m_TimeAcc = { 0.0 };
	m_UltTimeAcc = { 0.0 };
}

void ClintUltimate01::OnStateTick(_double TimeDelta)
{
	__super::OnStateTick(TimeDelta);

	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	CModel* pModel = static_cast<CModel*>(m_pOwner->Get_Component(L"Com_Model"));
	CTransform* pTransform = static_cast<CTransform*>(m_pOwner->Get_Component(L"Com_Transform"));
	ClintUltimate01Bullet* pUltBullet = static_cast<ClintUltimate01Bullet*>(m_pOwner->Get_Component(L"Com_"));
	_byte W = pGameInstance->Get_DIKeyState(DIK_W);
	_byte A = pGameInstance->Get_DIKeyState(DIK_A);
	_byte S = pGameInstance->Get_DIKeyState(DIK_S);
	_byte D = pGameInstance->Get_DIKeyState(DIK_D);

	Safe_Release(pGameInstance);

	m_TimeAcc += TimeDelta;
	if (m_TimeAcc >= m_Duration)
	{
		TransitionTo(L"ClintIdle");
	}

	if (W)
	{
		pTransform->Go_Direction(TimeDelta, CTransform::DIR_N);
	}
	else if (A)
	{
		pTransform->Go_Direction(TimeDelta, CTransform::DIR_W);
	}
	else if (S)
	{
		pTransform->Go_Direction(TimeDelta, CTransform::DIR_S);
	}
	else if (D)
	{
		pTransform->Go_Direction(TimeDelta, CTransform::DIR_E);
	}

	m_UltTimeAcc += TimeDelta;

	if (m_UltTimeAcc > 0.25)
	{
		m_UltTimeAcc = 0.0;
		ClintUltimate01Bullet* pUltBullet = static_cast<ClintUltimate01Bullet*>(m_pOwner->Get_Component(L"Com_UltBullet"));
		pUltBullet->Enable();
	}
}

void ClintUltimate01::OnStateExit()
{
	__super::OnStateExit();
}

StateMachine<Clint, CLINT_ANIM>* ClintUltimate01::Clone(void* pArg)
{
	__super::Initialize(pArg);
	return new ClintUltimate01(*this);
}

ClintUltimate01* ClintUltimate01::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	return new ClintUltimate01(pDevice, pContext);
}

void ClintUltimate01::Free()
{
	__super::Free();
}