#include "..\Public\PlayerHP.h"
#include "GameInstance.h"

PlayerHP::PlayerHP(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject(pDevice, pContext)
{
}

PlayerHP::PlayerHP(const PlayerHP& rhs)
	: CGameObject(rhs)
	, m_fRatio(1.f)
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
	_float2 fSize = ((tagPlayerHPDesc*)pArg)->Size;
	_float2 fOffsetPos = ((tagPlayerHPDesc*)pArg)->OffsetPos;

	m_pTransformCom->Scaled(_float3(fSize.x, fSize.y, 1.f));
	m_OffsetPos = _float4(fOffsetPos.x, fOffsetPos.y, 0.f, 1.f);
	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());
	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH(g_iWinSizeX, g_iWinSizeY, 0.f, 1.f));

	return S_OK;
}

void PlayerHP::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);
	_vector vOffsetPos = XMLoadFloat4(&m_OffsetPos);
	m_pTransformCom->SetUIPosition(_float2(640 + m_OffsetPos.x, 440 + m_OffsetPos.y));
	//_vector vPos = Facade->GetClintPosition() + vOffsetPos;
	//vPos.m128_f32[2] = 0.0f;
	//vPos.m128_f32[3] = 0.1f;
	
	//m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos);
}

void PlayerHP::Late_Tick(_double TimeDelta)
{
	__super::Late_Tick(TimeDelta);
	if (nullptr != m_pRendererCom)
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);
}

HRESULT PlayerHP::Render()
{
	FAILED_CHECK_RETURN(__super::Render(), E_FAIL);
	FAILED_CHECK_RETURN(SetUp_ShaderResources(), E_FAIL);
	FAILED_CHECK_RETURN(m_pShaderCom->Begin(1), E_FAIL);
	FAILED_CHECK_RETURN(m_pVIBufferCom->Render(), E_FAIL);

	return S_OK;
}

HRESULT PlayerHP::SetUp_ShaderResources()
{
	_float4x4 WorldMatrix = m_pTransformCom->Get_WorldFloat4x4();
	FAILED_CHECK_RETURN(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &WorldMatrix), E_FAIL);
	FAILED_CHECK_RETURN(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix), E_FAIL);
	FAILED_CHECK_RETURN(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix), E_FAIL);
	FAILED_CHECK_RETURN(m_pTextureCom->Bind_ShaderResources(m_pShaderCom, "g_Texture"), E_FAIL);

	return S_OK;
}

HRESULT PlayerHP::Add_Components()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);
	LEVELID eNextLevel = static_cast<LEVELID>(pGameInstance->Get_NextLevelIndex());
	/* For.Com_Renderer */
	CRenderer::CRENDERER_DESC tRendererDesc; tRendererDesc.pOwner = this;
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_STATIC, CRenderer::ProtoTag(), L"Com_Renderer", (CComponent**)&m_pRendererCom, &tRendererDesc), E_FAIL);

	/* For.Com_Transform */
	CTransform::CTRANSFORM_DESC TransformDesc{ 3.0, XMConvertToRadians(60.f) };	TransformDesc.pOwner = this;
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_STATIC, CTransform::ProtoTag(), L"Com_Transform", (CComponent**)&m_pTransformCom
		, &TransformDesc), E_FAIL);

	/* For.Com_VIBuffer_Rect */
	CVIBuffer_DynamicRect::tagCVIBuffer_DynamicRectDesc tVIBufferRectDesc; tVIBufferRectDesc.pOwner = this;
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_STATIC, CVIBuffer_DynamicRect::ProtoTag()
		, TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom, &tVIBufferRectDesc), E_FAIL);

	/* For.Com_Shader */
	CShader::CSHADER_DESC tShaderDesc; tShaderDesc.pOwner = this;
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_STATIC, L"Prototype_Component_Shader_Vtxtex"
		, L"Com_Shader", (CComponent**)&m_pShaderCom, &tShaderDesc), E_FAIL);

	/* For.Com_Texture */
	CTexture::CTEXTURE_DESC tTextureDesc; tTextureDesc.pOwner = this;
	FAILED_CHECK_RETURN(__super::Add_Component(eNextLevel
		, TEXT("Prototype_Component_Texture_PlayerHP"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom, &tTextureDesc), E_FAIL);
	
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

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
}


