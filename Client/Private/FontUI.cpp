
#include "FontUI.h"
#include "GameInstance.h"

_uint FontUI::FontUI_Id = 0;

FontUI::FontUI(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject(pDevice, pContext)

{
}

FontUI::FontUI(const FontUI& rhs)
	: CGameObject(rhs)
{
}

HRESULT FontUI::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}


HRESULT FontUI::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Add_Components()))
		return E_FAIL;

	++FontUI_Id;
	m_tInfo.wstrName = TO_WSTR("FontUI" + to_string(FontUI_Id));
	m_tInfo.wstrKey = ProtoTag();
	m_tInfo.ID = FontUI_Id;

	tagFontUIDesc tCloneDesc;
	if (nullptr != pArg)
		tCloneDesc = *(tagFontUIDesc*)pArg;
	tCloneDesc.vPosition.z = 0.f;
	tCloneDesc.vPosition.w = 1.f;
	SetPosition(_float2(tCloneDesc.vPosition.x, tCloneDesc.vPosition.y));
	m_fScale = tCloneDesc.fScale;

	return S_OK;
}

void FontUI::Late_Tick(_double TimeDelta)
{
	if (false == m_bEnable)
		return;

	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);




	Safe_Release(pGameInstance);

	__super::Late_Tick(TimeDelta);
	m_pRendererCom->Add_RenderGroup(m_eRenderGroup, this);
}

HRESULT FontUI::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);
	_vector vPos =  m_pTransformCom->GetPosition();
	_float fRotaion = m_pTransformCom->Get_AulerDegree();

	pGameInstance->Render_Font(m_Style.c_str(), m_Font.c_str()
		, _float2(vPos.m128_f32[0], vPos.m128_f32[1])
		, XMLoadFloat4(&m_vColor), fRotaion, m_vOrigin, m_fScale);

	Safe_Release(pGameInstance);
}

void FontUI::SetPosition(_float2 vPos)
{
	m_pTransformCom->SetPosition(XMVectorSet(vPos.x, g_iWinSizeY - vPos.y, 0.f, 1.f));
}

void FontUI::Go_Right(const _double& TimeDelta)
{
	m_pTransformCom->Go_Right(TimeDelta);
}

void FontUI::Go_Left(const _double& TimeDelta)
{
	m_pTransformCom->Go_Left(TimeDelta);
}

void FontUI::Go_Up(const _double& TimeDelta)
{
	m_pTransformCom->Go_Up(TimeDelta);
}

void FontUI::Go_Down(const _double& TimeDelta)
{
	m_pTransformCom->Go_Down(TimeDelta);
}

void FontUI::Rotation(_float fDegree)
{
	m_pTransformCom->Rotation(XMConvertToRadians(fDegree));
}

void FontUI::TurnCCW(const _double& TimeDelta)
{
	m_pTransformCom->TurnCCW(TimeDelta);
}

void FontUI::TurnCW(const _double& TimeDelta)
{
	m_pTransformCom->TurnCW(TimeDelta);
}

_bool FontUI::isInScreen()
{
	_vector vPos = m_pTransformCom->GetPosition();
	if (0 <= vPos.m128_f32[0] && vPos.m128_f32[0] <= g_iWinSizeX
		&& 0 <= vPos.m128_f32[1] && vPos.m128_f32[1] <= g_iWinSizeY)
		return true;
	return false;
}

HRESULT FontUI::Add_Components()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);
	LEVELID eLevelID = static_cast<LEVELID>(pGameInstance->Get_NextLevelIndex());

	CRenderer::CRENDERER_DESC tRendererDesc; tRendererDesc.pOwner = this;
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_STATIC, CRenderer::ProtoTag(), L"Com_Renderer", (CComponent**)&m_pRendererCom, &tRendererDesc), E_FAIL);

	CTransform2D::CTRANSFORM2D_DESC TransformDesc{ 7.0, XMConvertToRadians(160.f) }; TransformDesc.pOwner = this;
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_STATIC, CTransform2D::ProtoTag(), L"Com_Transform", (CComponent**)&m_pTransformCom
		, &TransformDesc), E_FAIL);

	Safe_Release(pGameInstance);
	return S_OK;
}

FontUI* FontUI::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	FontUI* pInstance = new FontUI(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created FontUI");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* FontUI::Clone(void* pArg)
{
	FontUI* pInstance = new FontUI(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned FontUI");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void FontUI::Free(void)
{
	__super::Free();

	--FontUI_Id;
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pRendererCom);
	/* Don't Forget Release for the VIBuffer or Model Component*/
}

