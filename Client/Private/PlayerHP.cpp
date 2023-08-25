#include "..\Public\PlayerHP.h"
#include "GameInstance.h"
#include "DynamicImage.h"
#include "Image.h"

PlayerHP::PlayerHP(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject(pDevice, pContext), HP()
{
}

PlayerHP::PlayerHP(const PlayerHP& rhs)
	: CGameObject(rhs), HP(rhs)
{

}

HRESULT PlayerHP::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT PlayerHP::Initialize(void* pArg)
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
	return S_OK;
}

void PlayerHP::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);
	m_pDynamicImage->Tick(TimeDelta);
	m_pImage->Tick(TimeDelta);
}

void PlayerHP::Late_Tick(_double TimeDelta)
{
	
	__super::Late_Tick(TimeDelta);
	m_pDynamicImage->Late_Tick(TimeDelta);
	m_pImage->Late_Tick(TimeDelta);
}

HRESULT PlayerHP::Render()
{

	return S_OK;
}

void PlayerHP::TakeDamage(_uint iAmount)
{
	SetAmount(iAmount);
	m_pObserver->Notify();
}

void PlayerHP::Heal(_uint iAmount)
{
	SetAmount(iAmount);
	m_pObserver->Notify();
}

HRESULT PlayerHP::SetUp_ShaderResources()
{

	return S_OK;
}

void PlayerHP::SetAmount(const _uint& iAmount)
{
	m_iAmount = iAmount;
}

HRESULT PlayerHP::Add_Components()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);
	LEVELID eLevelID = static_cast<LEVELID>(pGameInstance->Get_NextLevelIndex());

	DynamicImage::tagDynamicImageDesc tDynamicImageDesc;
	tDynamicImageDesc.Pos = _float2(g_iWinSizeX >> 1, (g_iWinSizeY >> 1) + 100);
	tDynamicImageDesc.Size = _float2(100, 16);
	tDynamicImageDesc.pTextureProtoTag = TEXT("Prototype_Component_Texture_LifeBar");
	__super::Add_Composite(DynamicImage::ProtoTag(), L"Com_LifeBar", (CComponent**)&m_pDynamicImage, &tDynamicImageDesc);

	Image::tagImageDesc tImageDesc;
	tImageDesc.Pos = _float2(g_iWinSizeX >> 1, (g_iWinSizeY >> 1) + 100);
	tImageDesc.Size = _float2(100, 16);
	tImageDesc.pTextureProtoTag = TEXT("Prototype_Component_Texture_LifeBar_BG");
	__super::Add_Composite(Image::ProtoTag(), L"Com_LifeBar_BG", (CComponent**)&m_pImage, &tImageDesc);

	Health::HEALTH_DESC tHealthDesc;
	tHealthDesc.pOwner = this;
	tHealthDesc.iMaxHp = 100;
	FAILED_CHECK_RETURN(__super::Add_Component(eLevelID, Health::ProtoTag(), L"Com_Health", (CComponent**)&m_pHealthCom, &tHealthDesc), E_FAIL);

	Observer::tagObserverDesc tObserverDesc;
	tObserverDesc.pOwner = this;
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_STATIC, Observer::ProtoTag(), L"Com_Observer", (CComponent**)&m_pObserver, &tObserverDesc), E_FAIL);

	Safe_Release(pGameInstance);
	return S_OK;
}

PlayerHP* PlayerHP::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	PlayerHP* pInstance = new PlayerHP(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created PlayerHP");
		Safe_Release(pInstance);
	}
	return pInstance;
}


CGameObject* PlayerHP::Clone(void* pArg)
{
	PlayerHP* pInstance = new PlayerHP(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned PlayerHP");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void PlayerHP::Free()
{
	__super::Free();

	Safe_Release(m_pDynamicImage);
	Safe_Release(m_pHealthCom);
	Safe_Release(m_pImage);
	Safe_Release(m_pObserver);
}


