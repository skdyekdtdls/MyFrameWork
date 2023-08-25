#include "SkillUI.h"
#include "GameInstance.h"
#include "FontUI.h"
#include "Image.h"
_uint SkillUI::SkillUI_Id = 0;

SkillUI::SkillUI(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject(pDevice, pContext)
	, m_fMaxCoolTime(0.f)
	, m_bIsRunning(false)
{
}

SkillUI::SkillUI(const SkillUI& rhs)
	: CGameObject(rhs)
	, m_fMaxCoolTime(rhs.m_fMaxCoolTime)
	, m_bIsRunning(rhs.m_bIsRunning)
{
}

HRESULT SkillUI::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT SkillUI::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Add_Components(*(tagSkillUIDesc*)pArg)))
		return E_FAIL;

	++SkillUI_Id;
	m_tInfo.wstrName = TO_WSTR("SkillUI" + to_string(SkillUI_Id));
	m_tInfo.wstrKey = ProtoTag();
	m_tInfo.ID = SkillUI_Id;

	m_pImage->SetRenderGroup(CRenderer::RENDER_UI_B);
	SetMaxCoolTime(((tagSkillUIDesc*)pArg)->fMaxCoolTime);
	m_pTimeCounter->Disable();
	m_pTimeCounter->Reset();
	m_pFontUI->SetColor(XMVectorSet(1.0f, 1.0f, 1.0f, 0.9f));
	// UI블랜드 논블랜드를 나눠줄지가 관건.

	return S_OK;
}

void SkillUI::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);
	m_pImage->Tick(TimeDelta);
	CoolTime();
	if (m_pTimeCounter->isGreaterThan(0.15f))
	{
		m_pFontUI->Tick(TimeDelta);
	}	
}

void SkillUI::Late_Tick(_double TimeDelta)
{
	__super::Late_Tick(TimeDelta);
	m_pImage->Late_Tick(TimeDelta);
	if (m_pTimeCounter->isGreaterThan(0.15f))
	{
		m_pFontUI->Late_Tick(TimeDelta);
	}
	m_pTimeCounter->Tick(TimeDelta);
}

void SkillUI::ImageDepth(_float Depth)
{
	Saturate(Depth, 0.f, 1.f);
	m_pImage->ImageDepth(Depth);
}

void SkillUI::UseSkill()
{
	if (true == m_bIsRunning)
		return;

	if (0.01f <= GetCoolTime())
		return;

	m_bIsRunning = true;
	m_pTimeCounter->Reset();
	m_pTimeCounter->Enable();
	m_pImage->SetPass(4);
}

HRESULT SkillUI::Add_Components(const tagSkillUIDesc& SkillDesc)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);
	LEVELID eLevelID = static_cast<LEVELID>(pGameInstance->Get_NextLevelIndex());

	Image::tagImageDesc tImageDesc;
	tImageDesc.pTextureProtoTag = SkillDesc.SkillTextureTag;
	tImageDesc.Pos = _float2(SkillDesc.vPosition.x, SkillDesc.vPosition.y);
	tImageDesc.Size = SkillDesc.fSize;
	FAILED_CHECK_RETURN(__super::Add_Composite(Image::ProtoTag(), L"Com_Image", (CComponent**)&m_pImage, &tImageDesc), E_FAIL);
	
	FontUI::tagFontUIDesc tFontDesc;
	tFontDesc.pOwner = this;
	tFontDesc.vPosition = SkillDesc.vPosition;
	tFontDesc.vPosition.x -= 35;
	tFontDesc.vPosition.y += 50;
	tFontDesc.fScale = 0.8f;
	FAILED_CHECK_RETURN(__super::Add_Composite(FontUI::ProtoTag(), L"Com_FontUI", (CComponent**)&m_pFontUI, &tFontDesc), E_FAIL);

	TimeCounter::tagTimeCounterDesc tTimeCounterDesc;
	tTimeCounterDesc.pOwner = this;
	FAILED_CHECK_RETURN(Add_Component(LEVEL_STATIC, TimeCounter::ProtoTag(), L"Com_TimeCounter", (CComponent**)&m_pTimeCounter, &tTimeCounterDesc), E_FAIL);

	Safe_Release(pGameInstance);

	return S_OK;
}

void SkillUI::CoolTime()
{
	if (false == m_bIsRunning)
		return;

	_double RoundValue = DRound(m_fMaxCoolTime - m_pTimeCounter->TimeElapse(), 1);
	m_pFontUI->Font((DbToWstr(RoundValue).c_str()));
	if (m_pTimeCounter->isEuqalWith(m_fMaxCoolTime))
	{
		m_pImage->SetPass(2);
		m_pTimeCounter->Disable();
		m_pTimeCounter->Reset();
		m_bIsRunning = false;
	}
}

SkillUI* SkillUI::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	SkillUI* pInstance = new SkillUI(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created SkillUI");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* SkillUI::Clone(void* pArg)
{
	SkillUI* pInstance = new SkillUI(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned SkillUI");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void SkillUI::Free(void)
{
	__super::Free();
	--SkillUI_Id;
	Safe_Release(m_pImage);
	Safe_Release(m_pFontUI);
	Safe_Release(m_pTimeCounter);
}


