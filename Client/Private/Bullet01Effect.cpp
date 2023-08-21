#include "Bullet01Effect.h"
#include "GameInstance.h"

_uint Bullet01Effect::Bullet01Effect_Id = 0;

Bullet01Effect::Bullet01Effect(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject(pDevice, pContext)
{
}

Bullet01Effect::Bullet01Effect(const Bullet01Effect& rhs)
	: CGameObject(rhs)
{
}

HRESULT Bullet01Effect::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}


HRESULT Bullet01Effect::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Add_Components()))
		return E_FAIL;

	++Bullet01Effect_Id;
	m_tInfo.wstrName = TO_WSTR("Bullet01Effect" + to_string(Bullet01Effect_Id));
	m_tInfo.wstrKey = ProtoTag();
	m_tInfo.ID = Bullet01Effect_Id;

	BULLET01_EFFECT_DESC tCloneDesc;
	if (nullptr != pArg)
		tCloneDesc = *(BULLET01_EFFECT_DESC*)pArg;
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMLoadFloat4(&tCloneDesc.vPosition));

	return S_OK;
}

void Bullet01Effect::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);

	// 	m_pModelCom->Play_Animation(TimeDelta);
	// 	if(nullptr != m_pColliderCom)
	//		m_pColliderCom->Tick(m_pTransformCom->Get_WorldMatrix());
}

void Bullet01Effect::Late_Tick(_double TimeDelta)
{
	__super::Late_Tick(TimeDelta);

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONLIGHT, this);
}

HRESULT Bullet01Effect::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	if (FAILED(SetUp_ShaderResources()))
		return E_FAIL;

	m_pShaderCom->Begin(0);
}

HRESULT Bullet01Effect::Add_Components()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);
	LEVELID eLevelID = static_cast<LEVELID>(pGameInstance->Get_NextLevelIndex());

	CRenderer::CRENDERER_DESC tRendererDesc; tRendererDesc.pOwner = this;
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_STATIC, CRenderer::ProtoTag(), L"Com_Renderer", (CComponent**)&m_pRendererCom, &tRendererDesc), E_FAIL);

	CTransform::CTRANSFORM_DESC TransformDesc{ 7.0, XMConvertToRadians(720.f) }; TransformDesc.pOwner = this;
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_STATIC, CTransform::ProtoTag(), L"Com_Transform", (CComponent**)&m_pTransformCom
		, &TransformDesc), E_FAIL);

	CShader::CSHADER_DESC tShaderDesc; tShaderDesc.pOwner = this;
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_STATIC, L"Prototype_Component_Shader_VtxPointInstance", L"Com_Shader", (CComponent**)&m_pShaderCom, &tShaderDesc), E_FAIL);

	Safe_Release(pGameInstance);
	return S_OK;
}

HRESULT Bullet01Effect::SetUp_ShaderResources()
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

	return S_OK;
}

Bullet01Effect* Bullet01Effect::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	Bullet01Effect* pInstance = new Bullet01Effect(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created Bullet01Effect");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* Bullet01Effect::Clone(void* pArg)
{
	Bullet01Effect* pInstance = new Bullet01Effect(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned Bullet01Effect");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void Bullet01Effect::Free(void)
{
	__super::Free();

	--Bullet01Effect_Id;
	
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pBufferCom);
	/* Don't Forget Release for the VIBuffer or Model Component*/
}

