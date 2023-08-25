#include "HitEffect.h"
#include "GameInstance.h"

_uint HitEffect::Effect4x4_Id = 0;

HitEffect::HitEffect(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject(pDevice, pContext)
{
}

HitEffect::HitEffect(const HitEffect& rhs)
	: CGameObject(rhs)
	, m_bEnable(false)
	, m_iCurrentIndex(-1)
{
}

HRESULT HitEffect::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT HitEffect::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	m_tEffectDesc = *(tagHitEffectDesc*)pArg;
	m_iMaxIndex = m_tEffectDesc.iRow * m_tEffectDesc.iCol;
	m_iLastIndex = m_iMaxIndex;

	if (FAILED(Add_Components()))
		return E_FAIL;

	++Effect4x4_Id;
	m_tInfo.wstrName = TO_WSTR("HitEffect" + to_string(Effect4x4_Id));
	m_tInfo.wstrKey = ProtoTag();
	m_tInfo.ID = Effect4x4_Id;

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMLoadFloat4(&m_tEffectDesc.vPosition));

	Reset();
	return S_OK;
}

void HitEffect::Tick(_double TimeDelta, _fvector vPos)
{
	if (false == m_bEnable)
		return;

	__super::Tick(TimeDelta);
	list<_float4x4>	ParticleMatrices;
	_float4x4 WorldMatrix;
	XMStoreFloat4x4(&WorldMatrix, XMMatrixIdentity());
	ParticleMatrices.push_back(WorldMatrix);
	m_pBufferCom->Tick(ParticleMatrices, TimeDelta);

	++m_iCurrentIndex;
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos);
	if (m_iCurrentIndex >= m_iLastIndex)
		m_bDead = true;
}

void HitEffect::Late_Tick(_double TimeDelta)
{
	if (false == m_bEnable)
		return;

	__super::Late_Tick(TimeDelta);
	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONLIGHT, this);
}

HRESULT HitEffect::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	if (FAILED(SetUp_ShaderResources()))
		return E_FAIL;

	// 만약에 모델 컴포넌트 안쓰면 이걸로 쓰면된다.
	m_pShaderCom->Begin(m_iPass);
	m_pBufferCom->Render();

	return S_OK;
}

void HitEffect::Reset()
{
	m_bEnable = true;
	m_iCurrentIndex = -1;
}

void HitEffect::Disable()
{
	m_bEnable = false;
}

HRESULT HitEffect::Add_Components()
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

	CTexture::CTEXTURE_DESC tTextureDesc; tTextureDesc.pOwner = this;
	FAILED_CHECK_RETURN(__super::Add_Component(eLevelID, m_tEffectDesc.pTextureTag, L"Com_Texture", (CComponent**)&m_pTextureCom, &tTextureDesc), E_FAIL);

	CVIBuffer_Point_Instance::tagCVIBuffer_Point_InstanceDesc tBufferDesc;
	tBufferDesc.pOwner = this;
	tBufferDesc.iNumInstance = 1;
	FAILED_CHECK_RETURN(Add_Component(eLevelID, CVIBuffer_Point_Instance::ProtoTag(), L"Com_Buffer"
		, (CComponent**)&m_pBufferCom, &tBufferDesc), E_FAIL);

	Safe_Release(pGameInstance);
	return S_OK;
}

HRESULT HitEffect::SetUp_ShaderResources()
{
	_float4x4 MyMatrix = m_pTransformCom->Get_WorldFloat4x4();
	FAILED_CHECK_RETURN(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &MyMatrix), E_FAIL);

	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	MyMatrix = pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_VIEW);
	FAILED_CHECK_RETURN(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &MyMatrix), E_FAIL);

	MyMatrix = pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_PROJ);
	FAILED_CHECK_RETURN(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &MyMatrix), E_FAIL);

	_float4 vCamPos = pGameInstance->Get_CamPositionFloat4();
	FAILED_CHECK_RETURN(m_pShaderCom->Bind_RawValue("g_vCamPosition", &vCamPos, sizeof(_float4)), E_FAIL);

	FAILED_CHECK_RETURN(m_pShaderCom->Bind_RawValue("g_iIndex", &m_iCurrentIndex, sizeof(_uint)), E_FAIL);
	FAILED_CHECK_RETURN(m_pShaderCom->Bind_RawValue("g_iRow", &m_tEffectDesc.iRow, sizeof(_uint)), E_FAIL);
	FAILED_CHECK_RETURN(m_pShaderCom->Bind_RawValue("g_iCol", &m_tEffectDesc.iCol, sizeof(_uint)), E_FAIL);
	FAILED_CHECK_RETURN(m_pShaderCom->Bind_RawValue("g_fSizeX", &m_fSizeX, sizeof(_float)), E_FAIL);
	FAILED_CHECK_RETURN(m_pShaderCom->Bind_RawValue("g_fSizeY", &m_fSizeY, sizeof(_float)), E_FAIL);
	FAILED_CHECK_RETURN(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture"), E_FAIL);

	Safe_Release(pGameInstance);

	return S_OK;
}

HitEffect* HitEffect::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	HitEffect* pInstance = new HitEffect(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created HitEffect");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* HitEffect::Clone(void* pArg)
{
	HitEffect* pInstance = new HitEffect(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned HitEffect");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void HitEffect::Free(void)
{
	__super::Free();

	--Effect4x4_Id;
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pBufferCom);
	/* Don't Forget Release for the VIBuffer or Model Component*/
}