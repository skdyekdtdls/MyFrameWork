#include "..\Public\BossHP.h"
#include "GameInstance.h"
#include "DynamicImage.h"
#include "Image.h"
#include "FontUI.h"
BossHP::BossHP(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject(pDevice, pContext)
{
}

BossHP::BossHP(const BossHP& rhs)
	: CGameObject(rhs)
{

}

HRESULT BossHP::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT BossHP::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Add_Components()))
		return E_FAIL;

	m_pDynamicImage->SetRatio(1.f);
	m_pObserver->Subscribe(L"TakeDamage", [this]() {
		m_pHealthCom->TakeDamage(m_iAmount);
		m_pDynamicImage->SetRatioLerp(m_pHealthCom->HPPercent() * 0.01f);
		});
	m_pImage->ImageDepth(0.0f);
	m_pDynamicImage->ImageDepth(0.001f);
	m_pHealthCom->SetMaxHP(1000);
	m_pFontUI->SetColor(XMVectorSet(1.0f, 0.84f, 0.0f, 1.0f));

	return S_OK;
}

void BossHP::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);
	m_pDynamicImage->Tick(TimeDelta);
	m_pImage->Tick(TimeDelta);
	m_pFontUI->Tick(TimeDelta);
}

void BossHP::Late_Tick(_double TimeDelta)
{
	__super::Late_Tick(TimeDelta);
	m_pDynamicImage->Late_Tick(TimeDelta);
	m_pImage->Late_Tick(TimeDelta);
	m_pFontUI->Late_Tick(TimeDelta);
}

HRESULT BossHP::Render()
{

	return S_OK;
}

void BossHP::TakeDamage(_uint iAmount)
{
	SetAmount(iAmount);
	m_pObserver->Notify();
}

void BossHP::Heal(_uint iAmount)
{
	SetAmount(iAmount);
	m_pObserver->Notify();
}

HRESULT BossHP::SetUp_ShaderResources()
{

	return S_OK;
}

void BossHP::SetAmount(const _uint& iAmount)
{
	m_iAmount = iAmount;
}

HRESULT BossHP::Add_Components()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);
	LEVELID eLevelID = static_cast<LEVELID>(pGameInstance->Get_NextLevelIndex());

	DynamicImage::tagDynamicImageDesc tDynamicImageDesc;
	tDynamicImageDesc.Pos = _float2(g_iWinSizeX >> 1, (g_iWinSizeY >> 1) + 200);
	tDynamicImageDesc.Size = _float2(1024, 64);
	tDynamicImageDesc.pTextureProtoTag = TEXT("Prototype_Component_Texture_Boss_HPBar");
	__super::Add_Composite(DynamicImage::ProtoTag(), L"Com_Boss_HPBar", (CComponent**)&m_pDynamicImage, &tDynamicImageDesc);

	Image::tagImageDesc tImageDesc;
	tImageDesc.Pos = tDynamicImageDesc.Pos;
	tImageDesc.Size = _float2(1024, 64);
	tImageDesc.pTextureProtoTag = TEXT("Prototype_Component_Texture_Boss_HP_BG");
	__super::Add_Composite(Image::ProtoTag(), L"Com_Boss_HP_BG", (CComponent**)&m_pImage, &tImageDesc);

	Health::HEALTH_DESC tHealthDesc;
	tHealthDesc.pOwner = this;
	tHealthDesc.iMaxHp = 1000;
	FAILED_CHECK_RETURN(__super::Add_Component(eLevelID, Health::ProtoTag(), L"Com_Health", (CComponent**)&m_pHealthCom, &tHealthDesc), E_FAIL);

	FontUI::tagFontUIDesc tFontDesc;
	tFontDesc.pOwner = this;
	tFontDesc.vPosition = _float4(tImageDesc.Pos.x - 100.f, g_iWinSizeY - tImageDesc.Pos.y - 100.f, 0.f, 1.f);
	tFontDesc.fScale = 1.f;
	__super::Add_Composite(FontUI::ProtoTag(), L"Com_FontUI", (CComponent**)&m_pFontUI, &tFontDesc);
	m_pFontUI->Font(L"¿©¿Õ ¸ð°í½º");

	Observer::tagObserverDesc tObserverDesc;
	tObserverDesc.pOwner = this;
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_STATIC, Observer::ProtoTag(), L"Com_Observer", (CComponent**)&m_pObserver, &tObserverDesc), E_FAIL);

	Safe_Release(pGameInstance);
	return S_OK;
}

BossHP* BossHP::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	BossHP* pInstance = new BossHP(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created BossHP");
		Safe_Release(pInstance);
	}
	return pInstance;
}


CGameObject* BossHP::Clone(void* pArg)
{
	BossHP* pInstance = new BossHP(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned BossHP");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void BossHP::Free()
{
	__super::Free();

	Safe_Release(m_pDynamicImage);
	Safe_Release(m_pHealthCom);
	Safe_Release(m_pImage);
	Safe_Release(m_pObserver);
	Safe_Release(m_pFontUI);
}


