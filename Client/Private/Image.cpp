#include "..\Public\Image.h"
#include "GameInstance.h"

Image::Image(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject(pDevice, pContext)
{
}

Image::Image(const Image& rhs)
	: CGameObject(rhs)
{

}

HRESULT Image::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT Image::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Add_Components(((tagImageDesc*)pArg)->pTextureProtoTag)))
		return E_FAIL;
	_float2 fSize = ((tagImageDesc*)pArg)->Size;
	_float2 fPos = ((tagImageDesc*)pArg)->Pos;

	m_pTransformCom->Scaled(_float3(fSize.x, fSize.y, 1.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(fPos.x - 0.5f * g_iWinSizeX
		, fPos.y - 0.5f * g_iWinSizeY, 0.01f, 1.f));

	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());
	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH(g_iWinSizeX, g_iWinSizeY, 0.f, 1.f));

	return S_OK;
}

void Image::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);
}

void Image::Late_Tick(_double TimeDelta)
{
	__super::Late_Tick(TimeDelta);
	if (nullptr != m_pRendererCom)
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);
}

HRESULT Image::Render()
{
	FAILED_CHECK_RETURN(__super::Render(), E_FAIL);
	FAILED_CHECK_RETURN(SetUp_ShaderResources(), E_FAIL);
	FAILED_CHECK_RETURN(m_pShaderCom->Begin(m_iPass), E_FAIL);
	FAILED_CHECK_RETURN(m_pVIBufferCom->Render(), E_FAIL);

	return S_OK;
}

void Image::ImageDepth(_float Depth)
{
	_vector vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	Saturate(Depth, 0.f, 1.f);
	vPos.m128_f32[2] = Depth;
	
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos);
}

HRESULT Image::SetUp_ShaderResources()
{
	_float4x4 WorldMatrix = m_pTransformCom->Get_WorldFloat4x4();
	FAILED_CHECK_RETURN(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &WorldMatrix), E_FAIL);
	FAILED_CHECK_RETURN(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix), E_FAIL);
	FAILED_CHECK_RETURN(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix), E_FAIL);
	FAILED_CHECK_RETURN(m_pTextureCom->Bind_ShaderResources(m_pShaderCom, "g_Texture"), E_FAIL);

	return S_OK;
}

HRESULT Image::Add_Components(const _tchar* pTextureProtoTag)
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
		, pTextureProtoTag, TEXT("Com_Texture"), (CComponent**)&m_pTextureCom, &tTextureDesc), E_FAIL);

	Safe_Release(pGameInstance);
	return S_OK;
}

Image* Image::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	Image* pInstance = new Image(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created Image");
		Safe_Release(pInstance);
	}
	return pInstance;
}


CGameObject* Image::Clone(void* pArg)
{
	Image* pInstance = new Image(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned Image");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void Image::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
}


