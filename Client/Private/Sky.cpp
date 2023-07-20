
#include "Sky.h"
#include "GameInstance.h"

_uint Sky::Sky_Id = 0;

/* Don't Forget Release for the VIBuffer or Model Component*/

Sky::Sky(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject(pDevice, pContext)
{
}

Sky::Sky(const Sky& rhs)
	: CGameObject(rhs)
{
}

HRESULT Sky::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}


HRESULT Sky::Initialize(void* pArg)
{
	//if (FAILED(__super::Initialize(pArg)))
	//	return E_FAIL;

	if (FAILED(Add_Components()))
		return E_FAIL;

	++Sky_Id;
	m_tInfo.wstrName = TO_WSTR("Sky" + to_string(Sky_Id));
	m_tInfo.wstrKey = ProtoTag();
	m_tInfo.ID = Sky_Id;

	SKY_DESC tSkyDesc;
	if (nullptr != pArg)
		tSkyDesc = *(SKY_DESC*)pArg;
	//m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMLoadFloat4(&tSkyDesc.vPosition));

	//m_pModelCom->Set_RootNode(3);

	ReleaseFreePickCollider();

	return S_OK;
}

void Sky::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);
}

void Sky::Late_Tick(_double TimeDelta)
{
	__super::Late_Tick(TimeDelta);

	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	_float4 vCamPosition = pGameInstance->Get_CamPosition();
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMLoadFloat4(&vCamPosition));

	Safe_Release(pGameInstance);
	
	if (nullptr != m_pRendererCom)
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_PRIORITY, this);
}

HRESULT Sky::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	if (FAILED(SetUp_ShaderResources()))
		return E_FAIL;

	m_pShaderCom->Begin(0);

	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT Sky::Add_Components()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);
	LEVELID eLevelID = static_cast<LEVELID>(pGameInstance->Get_NextLevelIndex());

	// no texture now, you have to add texture later

	CRenderer::CRENDERER_DESC tRendererDesc; tRendererDesc.pOwner = this;
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_STATIC, CRenderer::ProtoTag(), L"Com_Renderer", (CComponent**)&m_pRendererCom, &tRendererDesc), E_FAIL);
	// no texture now, you have to add texture later

	CTransform::CTRANSFORM_DESC TransformDesc{ 20.0, XMConvertToRadians(90.f) }; TransformDesc.pOwner = this;
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_STATIC, CTransform::ProtoTag(), L"Com_Transform", (CComponent**)&m_pTransformCom
		, &TransformDesc), E_FAIL);

	CVIBuffer_Cube::CVIBUFFER_CUBE_DESC tVIBufferCubeDesc; tVIBufferCubeDesc.pOwner = this;
	FAILED_CHECK_RETURN(__super::Add_Component(eLevelID, CVIBuffer_Cube::ProtoTag(), L"Com_VIBuffer", (CComponent**)&m_pVIBufferCom, &tVIBufferCubeDesc), E_FAIL);

	CShader::CSHADER_DESC tShaderDesc; tShaderDesc.pOwner = this;
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_STATIC, L"Prototype_Component_Shader_VtxCube", L"Com_Shader", (CComponent**)&m_pShaderCom, &tShaderDesc), E_FAIL);

	CTexture::CTEXTURE_DESC tTextureDesc; tTextureDesc.pOwner = this;
	FAILED_CHECK_RETURN(__super::Add_Component(eLevelID, L"Prototype_Component_Texture_Sky", L"Com_Texture", (CComponent**)&m_pTextureCom, &tTextureDesc), E_FAIL);

	Safe_Release(pGameInstance);
	return S_OK;
}

HRESULT Sky::SetUp_ShaderResources()
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

	FAILED_CHECK_RETURN(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", 2), E_FAIL);

	return S_OK;
}

Sky* Sky::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	Sky* pInstance = new Sky(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created Sky");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* Sky::Clone(void* pArg)
{
	Sky* pInstance = new Sky(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned Sky");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void Sky::Free(void)
{
	__super::Free();

	--Sky_Id;
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
}

