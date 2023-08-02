#include "..\Public\MonsterHP.h"
#include "GameInstance.h"
#include "DynamicImage.h"
#include "Image.h"

MonsterHP::MonsterHP(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject(pDevice, pContext)
{
}

MonsterHP::MonsterHP(const MonsterHP& rhs)
	: CGameObject(rhs)
{

}

HRESULT MonsterHP::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT MonsterHP::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Add_Components()))
		return E_FAIL;

	m_pDynamicImage->SetRatio(1.f);
	m_pObserver->Subscribe(L"TakeDamage", [this]() {
		m_pHealthCom->TakeDamage(m_iAmount);
		m_pDynamicImage->SetRatioLerp(m_pHealthCom->HPPercent() * 0.01f);
		Enable();
		});

	m_pImage->ImageDepth(0.0f);
	m_pDynamicImage->ImageDepth(0.001f);
	m_pDynamicImage->SetPass(1);
	m_pDynamicImage->SetRatio(1.f);
	m_pOwnerTransform = static_cast<CTransform*>(m_pOwner->Get_Component(L"Com_Transform"));

	Disable();
	return S_OK;
}

void MonsterHP::Tick(_double TimeDelta)
{
	if (false == m_bEnable)
		return;

	m_pTimeCounter->Tick(TimeDelta);
	if (m_pTimeCounter->isEuqalWith(3.0))
		Disable();

	__super::Tick(TimeDelta);
	m_pDynamicImage->Tick(TimeDelta);
	m_pImage->Tick(TimeDelta);

	_vector vPos = m_pOwnerTransform->Get_State(CTransform::STATE_POSITION);
	m_pDynamicImage->SetPosition(_float2(vPos.m128_f32[0], vPos.m128_f32[1]));
}

void MonsterHP::Late_Tick(_double TimeDelta)
{
	if (false == m_bEnable)
		return;

	__super::Late_Tick(TimeDelta);
	m_pDynamicImage->Late_Tick(TimeDelta);
	m_pImage->Late_Tick(TimeDelta);
}

void MonsterHP::TakeDamage(_uint iAmount)
{
	SetAmount(iAmount);
	m_pObserver->Notify();
}

void MonsterHP::Heal(_uint iAmount)
{
	SetAmount(iAmount);
	m_pObserver->Notify();
}

void MonsterHP::Enable()
{
	SetEnable(true);
	m_pTimeCounter->Enable();
	m_pTimeCounter->Reset();
	m_pDynamicImage->SetEnable(true);
}

void MonsterHP::Disable()
{
	SetEnable(false);
	m_pTimeCounter->Disable();	
	m_pDynamicImage->SetEnable(false);
}

void MonsterHP::SetAmount(const _uint& iAmount)
{
	m_iAmount = iAmount;
}

HRESULT MonsterHP::Add_Components()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);
	LEVELID eLevelID = static_cast<LEVELID>(pGameInstance->Get_NextLevelIndex());

	DynamicImage::tagDynamicImageDesc tDynamicImageDesc;
	tDynamicImageDesc.Pos = _float2(400, 400);
	tDynamicImageDesc.Size = _float2(100, 50);
	tDynamicImageDesc.pTextureProtoTag = TEXT("Prototype_Component_Texture_MonsterHpBar");
	__super::Add_Composite(DynamicImage::ProtoTag(), L"Com_MonsterHpBar", (CComponent**)&m_pDynamicImage, &tDynamicImageDesc);

	Image::tagImageDesc tImageDesc;
	tImageDesc.Pos = _float2(400, 400);
	tImageDesc.Size = _float2(100, 50);
	tImageDesc.pTextureProtoTag = TEXT("Prototype_Component_Texture_MonsterHp_BG");
	__super::Add_Composite(Image::ProtoTag(), L"Com_MonsterHp_BG", (CComponent**)&m_pImage, &tImageDesc);
	
	Health::HEALTH_DESC tHealthDesc;
	tHealthDesc.pOwner = this;
	tHealthDesc.iMaxHp = 1200;
	FAILED_CHECK_RETURN(__super::Add_Component(eLevelID, Health::ProtoTag(), L"Com_Health", (CComponent**)&m_pHealthCom, &tHealthDesc), E_FAIL);
	
	Observer::tagObserverDesc tObserverDesc;
	tObserverDesc.pOwner = this;
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_STATIC, Observer::ProtoTag(), L"Com_Observer", (CComponent**)&m_pObserver, &tObserverDesc), E_FAIL);
	
	TimeCounter::tagTimeCounterDesc tCounterDesc;
	tCounterDesc.pOwner = this;
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_STATIC, TimeCounter::ProtoTag(), L"Com_TimeCounter", (CComponent**)&m_pTimeCounter, &tCounterDesc), E_FAIL);

	Safe_Release(pGameInstance);
	return S_OK;
}

MonsterHP* MonsterHP::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	MonsterHP* pInstance = new MonsterHP(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created MonsterHP");
		Safe_Release(pInstance);
	}
	return pInstance;
}


CGameObject* MonsterHP::Clone(void* pArg)
{
	MonsterHP* pInstance = new MonsterHP(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned MonsterHP");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void MonsterHP::Free()
{
	__super::Free();

	Safe_Release(m_pDynamicImage);
	Safe_Release(m_pHealthCom);
	Safe_Release(m_pImage);
	Safe_Release(m_pObserver);
	Safe_Release(m_pTimeCounter);
}


