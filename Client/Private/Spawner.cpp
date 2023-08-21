#include "Spawner.h"
#include "GameInstance.h"
#include "Observer.h"
#include "Alien_prawn.h"
#include "BatPotato_RIG.h"
#include "CannonSpider.h"
#include "Spider.h"

_uint Spawner::Spawner_Id = 0;

Spawner::Spawner(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject(pDevice, pContext)
{
}

Spawner::Spawner(const Spawner& rhs)
	: CGameObject(rhs)
{
}

HRESULT Spawner::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	

	return S_OK;
}

HRESULT Spawner::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;
	
	Add_Components();
	Add_Event();

	++Spawner_Id;
	m_tInfo.wstrName = TO_WSTR("Spawner" + to_string(Spawner_Id));
	m_tInfo.wstrKey = ProtoTag();
	m_tInfo.ID = Spawner_Id;

	// ObjectPool<Alien_prawn>::GetInstance()->Resize(15);
	// ObjectPool<BatPotato_RIG>::GetInstance()->Resize(15);
	// ObjectPool<CannonSpider>::GetInstance()->Resize(15);
	// ObjectPool<Spider>::GetInstance()->Resize(15);

	return S_OK;
}

void Spawner::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);

	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	CNavigation* pClintNavigation = static_cast<CNavigation*>(pGameInstance->Get_ComponentOfClone(
		pGameInstance->Get_CurLevelIndex(), L"Layer_Player", "Clint1", L"Com_Navigation"
	));
	m_ClintCellIndex = pClintNavigation->GetCellCurIndex();
	Safe_Release(pGameInstance);

	m_pObserver->Notify();
}

void Spawner::Add_Event()
{
	m_pObserver->Subscribe(L"73_91", [this]() {
		if (73 == m_ClintCellIndex)
		{
			for (_uint i = 0; i < 10; ++i)
			{
				Alien_prawn::ALIEN_PRAWN_DESC tMonsterDesc;
				tMonsterDesc.vPosition = m_pNavigationCom->RandomPosInCell(91);
				tMonsterDesc.iStartIndex = 91;
				SpawnMonster<Alien_prawn>(&tMonsterDesc);
			}

			m_pObserver->UnSubscribeDelay(L"73_91");
		}
		});

	m_pObserver->Subscribe(L"TEST", [this]() {
		if (73 == m_ClintCellIndex)
		{
			for (_uint i = 0; i < 10; ++i)
			{
				BatPotato_RIG::BATPOTATO_RIG_DESC tMonsterDesc;
				tMonsterDesc.vPosition = m_pNavigationCom->RandomPosInCell(91);
				tMonsterDesc.iStartIndex = 91;
				SpawnMonster<BatPotato_RIG>(&tMonsterDesc);
			}

			for (_uint i = 0; i < 10; ++i)
			{
				CannonSpider::tagCannonSpiderDesc tMonsterDesc;
				tMonsterDesc.vPosition = m_pNavigationCom->RandomPosInCell(91);
				tMonsterDesc.iStartIndex = 91;
				SpawnMonster<CannonSpider>(&tMonsterDesc);
			}

			for (_uint i = 0; i < 10; ++i)
			{
				Spider::tagSpiderDesc tMonsterDesc;
				tMonsterDesc.vPosition = m_pNavigationCom->RandomPosInCell(91);
				tMonsterDesc.iStartIndex = 91;
				SpawnMonster<Spider>(&tMonsterDesc);
			}

			m_pObserver->UnSubscribeDelay(L"TEST");
		}
		});
}

HRESULT Spawner::Add_Components()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);
	LEVELID eLevelID = static_cast<LEVELID>(pGameInstance->Get_NextLevelIndex());

	Observer::tagObserverDesc tObserverDesc;
	tObserverDesc.pOwner = this;
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_STATIC, Observer::ProtoTag(), L"Com_Observer", (CComponent**)&m_pObserver, &tObserverDesc), E_FAIL);

	CNavigation::CNAVIGATION_DESC tNavigationdesc; tNavigationdesc.pOwner = this;
	tNavigationdesc.iCurrentIndex = { 0 };
	FAILED_CHECK_RETURN(__super::Add_Component(eLevelID, CNavigation::ProtoTag(), L"Com_Navigation", (CComponent**)&m_pNavigationCom, &tNavigationdesc), E_FAIL);

	Safe_Release(pGameInstance);
	return S_OK;
}

Spawner* Spawner::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	Spawner* pInstance = new Spawner(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created Spawner");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* Spawner::Clone(void* pArg)
{
	Spawner* pInstance = new Spawner(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned Spawner");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void Spawner::Free(void)
{
	__super::Free();
	Safe_Release(m_pObserver);
	Safe_Release(m_pNavigationCom);
	--Spawner_Id;

	/* Don't Forget Release for the VIBuffer or Model Component*/
}

