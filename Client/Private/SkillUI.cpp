#include "SkillUI.h"
#include "GameInstance.h"
#include "FontUI.h"
#include "Image.h"
_uint SkillUI::SkillUI_Id = 0;

SkillUI::SkillUI(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject(pDevice, pContext)
{
}

SkillUI::SkillUI(const SkillUI& rhs)
	: CGameObject(rhs)
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
	m_pImage->ImageDepth(0.1f); // 뭔가 알파블랜드 왜곡문제가 발생한다.
	// UI블랜드 논블랜드를 나눠줄지가 관건.
	return S_OK;
}

void SkillUI::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);
	m_pImage->Tick(TimeDelta);

	if (0.01f <= m_fCoolTime)
	{
		m_pFontUI->Tick(TimeDelta);
	}
	
	//m_pFontUI->Font(to_wstring(m_iLevel).c_str());
}

void SkillUI::Late_Tick(_double TimeDelta)
{
	__super::Late_Tick(TimeDelta);
	m_pImage->Late_Tick(TimeDelta);
	m_pFontUI->Late_Tick(TimeDelta);
}

void SkillUI::ImageDepth(_float Depth)
{
	Saturate(Depth, 0.f, 1.f);
	m_pImage->ImageDepth(Depth);
}

void SkillUI::CoolTime()
{
	m_fCoolTime = m_fMaxCoolTime;
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
	__super::Add_Composite(Image::ProtoTag(), L"Com_Image", (CComponent**)&m_pImage, &tImageDesc);
	
	FontUI::tagFontUIDesc tFontDesc;
	tFontDesc.pOwner = this;
	tFontDesc.vPosition = SkillDesc.vPosition;
	tFontDesc.fScale = 1.f;
	__super::Add_Composite(FontUI::ProtoTag(), L"Com_FontUI", (CComponent**)&m_pFontUI, &tFontDesc);

	Safe_Release(pGameInstance);
	return S_OK;
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
}

