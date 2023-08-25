
#include "CustomMouse.h"
#include "GameInstance.h"
#include "Image.h"
_uint CustomMouse::CustomMouse_Id = 0;

CustomMouse::CustomMouse(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject(pDevice, pContext)
{
}

CustomMouse::CustomMouse(const CustomMouse& rhs)
	: CGameObject(rhs)
{
}

HRESULT CustomMouse::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	

	return S_OK;
}

HRESULT CustomMouse::Initialize(void* pArg)
{
	Add_Components();

	return S_OK;
}

void CustomMouse::Tick(_double TimeDelta)
{
	::POINT	ptMouse{};
	GetCursorPos(&ptMouse);
	ScreenToClient(g_hWnd, &ptMouse);
	m_pImageCom->SetPosition(ptMouse.x, g_iWinSizeY - ptMouse.y);
}

void CustomMouse::Late_Tick(_double TimeDelta)
{
	__super::Late_Tick(TimeDelta);

	m_pImageCom->Late_Tick(TimeDelta);
}

HRESULT CustomMouse::Render()
{
	//if (FAILED(__super::Render()))
	//	return E_FAIL;

	//if (FAILED(SetUp_ShaderResources()))
	//	return E_FAIL;

	//m_pShaderCom->Begin(1);
	//m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CustomMouse::Add_Components()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	//Safe_AddRef(pGameInstance);
	//LEVELID eLevelID = static_cast<LEVELID>(pGameInstance->Get_NextLevelIndex());

	//CRenderer::CRENDERER_DESC tRendererDesc; tRendererDesc.pOwner = this;
	//FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_STATIC, CRenderer::ProtoTag(), L"Com_Renderer", (CComponent**)&m_pRendererCom, &tRendererDesc), E_FAIL);

	//CTransform2D::tagCTransform2DDesc TransformDesc{ 7.0, XMConvertToRadians(720.f) }; TransformDesc.pOwner = this;
	//FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_STATIC, CTransform2D::ProtoTag(), L"Com_Transform", (CComponent**)&m_pTransformCom
	//	, &TransformDesc), E_FAIL);

	//CShader::CSHADER_DESC tShaderDesc; tShaderDesc.pOwner = this;
	//FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_STATIC, L"Prototype_Component_Shader_Vtxtex", L"Com_Shader", (CComponent**)&m_pShaderCom, &tShaderDesc), E_FAIL);

	//CTexture::tagCTextureDesc tTextureDesc; tTextureDesc.pOwner = this;
	//FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_STATIC, L"Prototype_Component_Texture_Mouse", L"Com_Texture", (CComponent**)&m_pTexutreCom, &tTextureDesc), E_FAIL);

	//CVIBuffer_Rect::tagCVIBufferDesc tBufferDesc; tBufferDesc.pOwner = this;
	//FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_STATIC, CVIBuffer_Rect::ProtoTag(), L"Com_Buffer", (CComponent**)&m_pVIBufferCom, &tBufferDesc), E_FAIL);

	Image::tagImageDesc tImageDESC;
	tImageDESC.Pos = _float2(g_iWinSizeX >> 1, g_iWinSizeY >> 1);
	tImageDESC.pTextureProtoTag = L"Prototype_Component_Texture_Mouse";
	tImageDESC.Size = _float2(72.f, 72.f);
	FAILED_CHECK_RETURN(__super::Add_Composite(Image::ProtoTag(), L"Com_IMage", (CComponent**)&m_pImageCom, &tImageDESC), E_FAIL);

	Safe_Release(pGameInstance);
	return S_OK;
}

HRESULT CustomMouse::SetUp_ShaderResources()
{
	//_float4x4 MyMatrix = m_pTransformCom->Get_WorldFloat4x4();
	//FAILED_CHECK_RETURN(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &MyMatrix), E_FAIL);

	//CGameInstance* pGameInstance = CGameInstance::GetInstance();
	//Safe_AddRef(pGameInstance);

	//MyMatrix = pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_VIEW);
	//FAILED_CHECK_RETURN(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix), E_FAIL);

	//MyMatrix = pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_PROJ);
	//FAILED_CHECK_RETURN(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix), E_FAIL);

	//FAILED_CHECK_RETURN(m_pTexutreCom->Bind_ShaderResources(m_pShaderCom, "g_Texture"), E_FAIL);

	//Safe_Release(pGameInstance);

	return S_OK;
}

CustomMouse* CustomMouse::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CustomMouse* pInstance = new CustomMouse(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created CustomMouse");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CustomMouse::Clone(void* pArg)
{
	CustomMouse* pInstance = new CustomMouse(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned CustomMouse");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CustomMouse::Free(void)
{
	__super::Free();

	//--CustumMouse_Id;
	//Safe_Release(m_pTexutreCom);
	//Safe_Release(m_pShaderCom);
	//Safe_Release(m_pVIBufferCom);
	//Safe_Release(m_pRendererCom);
	//Safe_Release(m_pTransformCom);

	Safe_Release(m_pImageCom);

	/* Don't Forget Release for the VIBuffer or Model Component*/
}

