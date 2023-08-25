#include "PlayerLevel.h"
#include "GameInstance.h"
#include "DynamicImage.h"
#include "FontUI.h"
_uint PlayerLevel::PlayerLevel_Id = 0;

PlayerLevel::PlayerLevel(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject(pDevice, pContext)
{
}

PlayerLevel::PlayerLevel(const PlayerLevel& rhs)
	: CGameObject(rhs)
{
}

HRESULT PlayerLevel::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT PlayerLevel::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Add_Components(((tagPlayerLevelDesc*)pArg)->vPosition, ((tagPlayerLevelDesc*)pArg)->fScale)))
		return E_FAIL;

	++PlayerLevel_Id;
	m_tInfo.wstrName = TO_WSTR("PlayerLevel" + to_string(PlayerLevel_Id));
	m_tInfo.wstrKey = ProtoTag();
	m_tInfo.ID = PlayerLevel_Id;

	tagPlayerLevelDesc tCloneDesc;
	if (nullptr != pArg)
		tCloneDesc = *(tagPlayerLevelDesc*)pArg;
	
	m_pDynamicImage->SetRatio(0.f);

	return S_OK;
}

void PlayerLevel::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);
}

void PlayerLevel::Late_Tick(_double TimeDelta)
{
	__super::Late_Tick(TimeDelta);
	m_pDynamicImage->Late_Tick(TimeDelta);
	m_pFontUI->Late_Tick(TimeDelta);
}

void PlayerLevel::LevelUp()
{
	++m_iLevel;
	
	if (m_iLevel >= 10)
	{
		m_iLevel = 9;
		m_bNoLevelUp = true;
	}
	m_pFontUI->Font(to_wstring(m_iLevel).c_str());
}

void PlayerLevel::AddExp(_float fExp)
{
	if (m_bNoLevelUp)
	{
		return;
	}

	m_fExp += fExp;

	if (m_fExp >= m_fMaxExp)
	{
		LevelUp();

		if (m_bNoLevelUp)
			m_fExp = 99.99f;
		else
			m_fExp = 0.f;
	}
	
	m_pDynamicImage->SetRatioLerp(m_fExp / m_fMaxExp);
}

HRESULT PlayerLevel::Add_Components(_float4 vPosition, _float fScale)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);
	LEVELID eLevelID = static_cast<LEVELID>(pGameInstance->Get_NextLevelIndex());

	CRenderer::CRENDERER_DESC tRendererDesc; tRendererDesc.pOwner = this;
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_STATIC, CRenderer::ProtoTag(), L"Com_Renderer", (CComponent**)&m_pRendererCom, &tRendererDesc), E_FAIL);

	DynamicImage::tagDynamicImageDesc tDynamicImageDesc;
	tDynamicImageDesc.Pos = _float2(652, 37);
	//tDynamicImageDesc.Pos = _float2(652, 400);
	tDynamicImageDesc.Size = _float2(280, 14);
	tDynamicImageDesc.pTextureProtoTag = TEXT("Prototype_Component_Texture_ExpBar");
	__super::Add_Composite(DynamicImage::ProtoTag(), L"Com_DynamicImage", (CComponent**)&m_pDynamicImage, &tDynamicImageDesc);

	FontUI::tagFontUIDesc tFontDesc;
	tFontDesc.pOwner = this;
	tFontDesc.vPosition = vPosition;
	tFontDesc.fScale = fScale;
	__super::Add_Composite(FontUI::ProtoTag(), L"Com_FontUI", (CComponent**)&m_pFontUI, &tFontDesc);
	m_pFontUI->Font(to_wstring(m_iLevel).c_str());
	Safe_Release(pGameInstance);
	return S_OK;
}

PlayerLevel* PlayerLevel::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	PlayerLevel* pInstance = new PlayerLevel(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created PlayerLevel");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* PlayerLevel::Clone(void* pArg)
{
	PlayerLevel* pInstance = new PlayerLevel(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned PlayerLevel");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void PlayerLevel::Free(void)
{
	__super::Free();

	--PlayerLevel_Id;
	Safe_Release(m_pDynamicImage);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pFontUI);
	/* Don't Forget Release for the VIBuffer or Model Component*/
}
