#include "..\Public\Blue_Snow.h"
#include "GameInstance.h"

CBlue_Snow::CBlue_Snow(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject(pDevice, pContext)
{
}

CBlue_Snow::CBlue_Snow(const CBlue_Snow& rhs)
	: CGameObject(rhs)
{
}

HRESULT CBlue_Snow::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CBlue_Snow::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Add_Components()))
		return E_FAIL;

	return S_OK;
}

void CBlue_Snow::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);

	m_pVIBufferCom->Tick(TimeDelta);
}

void CBlue_Snow::Late_Tick(_double TimeDelta)
{
	__super::Late_Tick(TimeDelta);

	if (Single->isRender(m_pRendererCom, m_pTransformCom, 5.f))
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONLIGHT, this);
}

HRESULT CBlue_Snow::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	if (FAILED(SetUp_ShaderResources()))
		return E_FAIL;

	m_pShaderCom->Begin(0);

	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CBlue_Snow::Add_Components()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);
	_uint iNextLevel = pGameInstance->Get_NextLevelIndex();

	/* For.Com_Renderer */
	CRenderer::CRENDERER_DESC tRendererDesc; tRendererDesc.pOwner = this;
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_STATIC, CRenderer::ProtoTag(), L"Com_Renderer", (CComponent**)&m_pRendererCom, &tRendererDesc), E_FAIL);


	/* For.Com_Transform */
	CTransform::CTRANSFORM_DESC TransformDesc{ 7.0, XMConvertToRadians(90) }; TransformDesc.pOwner = this;
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_STATIC, CTransform::ProtoTag(), L"Com_Transform", (CComponent**)&m_pTransformCom
		, &TransformDesc), E_FAIL);

	/* For.Com_VIBuffer */
	FAILED_CHECK_RETURN(__super::Add_Component(iNextLevel, CVIBuffer_Rect_Instance::ProtoTag(), L"Com_VIBuffer", (CComponent**)&m_pVIBufferCom
		, nullptr), E_FAIL);

	/* For.Com_Shader */
	CShader::CSHADER_DESC tShaderDesc; tShaderDesc.pOwner = this;
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxTexInstance"),
		TEXT("Com_Shader"), (CComponent**)&m_pShaderCom, &tShaderDesc), E_FAIL);

	/* For.Com_Texture */
	CTexture::CTEXTURE_DESC tTextureDesc; tTextureDesc.pOwner = this;
	FAILED_CHECK_RETURN(__super::Add_Component(iNextLevel, TEXT("Prototype_Component_Texture_Snow"),
		TEXT("Com_Texture"), (CComponent**)&m_pTextureCom, &tTextureDesc), E_FAIL);

	Safe_Release(pGameInstance);
	return S_OK;
}

HRESULT CBlue_Snow::SetUp_ShaderResources()
{
	_float4x4 MyMatrix = m_pTransformCom->Get_WorldFloat4x4();
	FAILED_CHECK_RETURN(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &MyMatrix), E_FAIL);

	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	MyMatrix = pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_VIEW);
	FAILED_CHECK_RETURN(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &MyMatrix), E_FAIL);

	MyMatrix = pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_PROJ);
	FAILED_CHECK_RETURN(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &MyMatrix), E_FAIL);

	Safe_Release(pGameInstance);

	FAILED_CHECK_RETURN(m_pTextureCom->Bind_ShaderResources(m_pShaderCom, "g_Texture"), E_FAIL);
	return S_OK;
}

CBlue_Snow* CBlue_Snow::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CBlue_Snow* pInstance = new CBlue_Snow(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created CBlue_Snow");
		Safe_Release(pInstance);
	}
	return pInstance;
}


CGameObject* CBlue_Snow::Clone(void* pArg)
{
	CBlue_Snow* pInstance = new CBlue_Snow(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned CBlue_Snow");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CBlue_Snow::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);

}
