#include "BoomEffect.h"
#include "GameInstance.h"

BoomEffect::BoomEffect(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject(pDevice, pContext)
{

}

BoomEffect::BoomEffect(const BoomEffect& rhs)
	: CGameObject(rhs)
{

}

HRESULT BoomEffect::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	m_iNumParticles = ((BOOM_EFFECT_DESC*)pArg)->iNumParticles;
	m_Particles.resize(m_iNumParticles);
	if (FAILED(Add_Components(((BOOM_EFFECT_DESC*)pArg)->pTextureProtoTag)))
		return E_FAIL;

	Reset_Effects();

	return S_OK;
}

void BoomEffect::Tick(_double TimeDelta)
{
	list<_float4x4>		ParticleMatrices;
	for (auto& Particle : m_Particles)
	{
		//Particle.dAge += TimeDelta;

		//if (Particle.dAge > Particle.dLifeTime /*+ Particle.dGenTime*/)
		//	Particle.isAlive = false;

		_float4 vPos;
		_vector vVelocity;

		// 이전프레임의 위치값을 가져옴.
		memcpy(&vPos, Particle.WorldMatrix.m[3], sizeof(_float4));

		// 가속도를 이용해 최종 속도를 정함.
		vVelocity = XMLoadFloat4(&Particle.vVelocity);
		//vVelocity += XMLoadFloat4(&Particle.vAccel) * TimeDelta;
		//XMStoreFloat4(&Particle.vVelocity, vVelocity);

		// 위치에 속도를 더해서 최종 위치를 정함.
		XMStoreFloat4(&vPos, XMLoadFloat4(&vPos) + vVelocity * _float(TimeDelta));

		// SRT 연산
		XMStoreFloat4x4(&Particle.WorldMatrix, XMMatrixScaling(0.5f, 0.5f, 0.5f) * XMMatrixTranslation(vPos.x, vPos.y, vPos.z));

		ParticleMatrices.push_back(Particle.WorldMatrix);
	}

	m_pBufferCom->Tick(ParticleMatrices, TimeDelta);
}

void BoomEffect::Late_Tick(_double TimeDelta)
{
	if (nullptr != m_pRendererCom)
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONLIGHT, this);
}

HRESULT BoomEffect::Render()
{
	if (FAILED(SetUp_ShaderResources()))
		return E_FAIL;

	m_pShaderCom->Begin(3);

	m_pBufferCom->Render();

	return S_OK;
}

void BoomEffect::Reset_Effects()
{
	for (auto& particle : m_Particles)
	{
		Reset_Particle(particle);
	}
}

void BoomEffect::Reset_Particle(BOOMPARTICLE& Particle)
{
	Particle.dAge = 0.f;
	Particle.dLifeTime = 2.f;
	Particle.isAlive = true;

	_float fSpeed = 15.f;
	Particle.vVelocity = _float4(RandomFloat(-fSpeed, fSpeed), RandomFloat(-fSpeed, fSpeed), RandomFloat(-fSpeed, fSpeed), 0.f);

	//Particle.vAccel = _float4(0.f, RandomFloat(-35.f, -45.f), 0.f, 0.f);

	XMStoreFloat4x4(&Particle.WorldMatrix, XMMatrixIdentity());
}

void BoomEffect::Render_Effect(_fvector vEffectPos)
{
	Reset_Effects();

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vEffectPos);
}

HRESULT BoomEffect::Add_Components(const _tchar* TextureProtoTag)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);
	LEVELID eNextLevel = static_cast<LEVELID>(pGameInstance->Get_NextLevelIndex());

	/* For.Com_Renderer */
	CRenderer::CRENDERER_DESC tRendererDesc; tRendererDesc.pOwner = this;
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_STATIC, CRenderer::ProtoTag(), L"Com_Renderer", (CComponent**)&m_pRendererCom, &tRendererDesc), E_FAIL);

	/* For.Com_Transform */
	CTransform::CTRANSFORM_DESC TransformDesc{ 7.0, XMConvertToRadians(90) }; TransformDesc.pOwner = this;
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_STATIC, CTransform::ProtoTag(), L"Com_Transform", (CComponent**)&m_pTransformCom
		, &TransformDesc), E_FAIL);

	/* For.Com_Texture */
	CTexture::tagCTextureDesc tTextureDesc;
	tTextureDesc.pOwner = this;
	FAILED_CHECK_RETURN(__super::Add_Component(eNextLevel, TextureProtoTag
		, L"Com_Texture", (CComponent**)&m_pTextureCom, &tTextureDesc), E_FAIL);

	CTexture::tagCTextureDesc tTextureGraDesc;
	tTextureGraDesc.pOwner = this;
	FAILED_CHECK_RETURN(__super::Add_Component(eNextLevel, L"Prototype_Component_Texture_OneGradient"
		, L"Com_TextureGra", (CComponent**)&m_pTextureGraCom, &tTextureGraDesc), E_FAIL);

	/* For.Com_Shader */
	CShader::CSHADER_DESC tShaderDesc; tShaderDesc.pOwner = this;
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxPointInstance"),
		TEXT("Com_Shader"), (CComponent**)&m_pShaderCom, &tShaderDesc), E_FAIL);

	/* For.Com_VIBuffer*/
	CVIBuffer_Point_Instance::CVIBUFFER_POINT_INSTANCE_DESC tDesc;
	tDesc.pOwner = this;
	tDesc.iNumInstance = m_iNumParticles;
	FAILED_CHECK_RETURN(Add_Component(eNextLevel, CVIBuffer_Point_Instance::ProtoTag(), L"Com_Buffer"
		, (CComponent**)&m_pBufferCom, &tDesc), E_FAIL);

	Safe_Release(pGameInstance);
	return S_OK;
}

HRESULT BoomEffect::SetUp_ShaderResources()
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

	Safe_Release(pGameInstance);

	FAILED_CHECK_RETURN(m_pTextureCom->Bind_ShaderResources(m_pShaderCom, "g_Texture"), E_FAIL);
	FAILED_CHECK_RETURN(m_pTextureGraCom->Bind_ShaderResources(m_pShaderCom, "g_TextureGradient"), E_FAIL);

	return S_OK;
}


BoomEffect* BoomEffect::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	BoomEffect* pInstance = new BoomEffect(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created BoomEffect");
		Safe_Release(pInstance);
	}
	return pInstance;
}

BoomEffect* BoomEffect::Clone(void* pArg)
{
	BoomEffect* pInstance = new BoomEffect(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned BoomEffect");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void BoomEffect::Free()
{
	__super::Free();
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTextureGraCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pBufferCom);
	Safe_Release(m_pTransformCom);
	m_Particles.clear();
}
