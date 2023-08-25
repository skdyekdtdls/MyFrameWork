#include "PropelEffect.h"
#include "GameInstance.h"

PropelEffect::PropelEffect(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject(pDevice, pContext)
{

}

PropelEffect::PropelEffect(const PropelEffect& rhs)
	: CGameObject(rhs)
{

}

HRESULT PropelEffect::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	m_iNumParticles = ((PROPEL_EFFECT_DESC*)pArg)->iNumParticles;
	m_iNumParticles = 50;
	m_Particles.resize(m_iNumParticles);
	if (FAILED(Add_Components(((PROPEL_EFFECT_DESC*)pArg)->pTextureProtoTag)))
		return E_FAIL;

	m_pOwnerTransform = static_cast<CTransform*>(m_pOwner->Get_Component(L"Com_Transform"));

	Reset_Effects();

	return S_OK;
}

void PropelEffect::Tick(_double TimeDelta)
{
	list<_float4x4> ParticleMatrices;
	_matrix matScale, matTrans;

	for (auto& Particle : m_Particles)
	{
		Particle.dAge += TimeDelta;

		if (Particle.dAge > Particle.dLifeTime)
		{
			Reset_Particle(Particle);
		}

		if (Particle.dAge > Particle.dGenTime && false == Particle.bStart)
		{
			Particle.bStart = true;
			_vector vPosition = m_pOwnerTransform->Get_State(CTransform::STATE_POSITION);
			_float4 vPositionf;
			XMStoreFloat4(&vPositionf, vPosition);
			memcpy(&Particle.WorldMatrix.m[3], &vPositionf, sizeof(_float4));
		}

		// 크기를 조절
		matScale = XMMatrixScaling(0.2f, 0.2f, 0.2f);

		// 파티클의 최종 위치를 계산합니다.
		_float4 vPos;
		memcpy(&vPos, Particle.WorldMatrix.m[3], sizeof(_float4));
		_vector vDirection = XMLoadFloat4(&Particle.vDirection);
		XMStoreFloat4(&vPos, XMLoadFloat4(&vPos) + vDirection * Particle.fSpeed * _float(TimeDelta));
		matTrans = XMMatrixTranslation(vPos.x, vPos.y, vPos.z);

		// SRT 연산으로 파티클의 월드 행렬을 갱신합니다.
		XMStoreFloat4x4(&Particle.WorldMatrix, matScale * matTrans);

		ParticleMatrices.push_back(Particle.WorldMatrix);
	}

	m_pBufferCom->Tick(ParticleMatrices, TimeDelta);
}


void PropelEffect::Late_Tick(_double TimeDelta)
{
	if (nullptr != m_pRendererCom)
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONLIGHT, this);
}

HRESULT PropelEffect::Render()
{
	if (FAILED(SetUp_ShaderResources()))
		return E_FAIL;

	m_pShaderCom->Begin(2);

	m_pBufferCom->Render();

	return S_OK;
}

void PropelEffect::Reset_Effects()
{
	for (auto& particle : m_Particles)
	{
		Reset_Particle(particle);
	}
}

void PropelEffect::Reset_Particle(PROPELPARTICLE& Particle)
{
	Particle.dAge = 0.f;
	Particle.dLifeTime = RandomFloat(0.5f, 1.f);
	Particle.dGenTime = RandomFloat(0.f, Particle.dLifeTime * 0.5f);
	Particle.fSpeed = RandomFloat(0.f, 3.f);
	Particle.bStart = false;
	XMStoreFloat4(&Particle.vDirection, RandomDirection());
	//Particle.vVelocity = RandomFloat4(_float4(0.f, 0.f, 0.f, 0.f), _float4(2.f, 2.f, 2.f, 0.f));
	//_float fNoiseScale = 0.2f;
	//_float4 vNoise = RandomFloat4(_float4(-fNoiseScale, -fNoiseScale, -fNoiseScale, 0.f)
    //		, _float4(fNoiseScale, fNoiseScale, fNoiseScale, 0.f));
	//XMStoreFloat4(&Particle.vDirection, XMVector4Normalize(-1.f * vDirection + XMLoadFloat4(&vNoise)));

	XMStoreFloat4x4(&Particle.WorldMatrix, XMMatrixIdentity());
}

void PropelEffect::Render_Effect(_fvector vEffectPos)
{
	Reset_Effects();

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vEffectPos);
}

HRESULT PropelEffect::Add_Components(const _tchar* TextureProtoTag)
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

HRESULT PropelEffect::SetUp_ShaderResources()
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

	return S_OK;
}


PropelEffect* PropelEffect::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	PropelEffect* pInstance = new PropelEffect(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created PropelEffect");
		Safe_Release(pInstance);
	}
	return pInstance;
}

PropelEffect* PropelEffect::Clone(void* pArg)
{
	PropelEffect* pInstance = new PropelEffect(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned PropelEffect");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void PropelEffect::Free()
{
	__super::Free();
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pBufferCom);
	Safe_Release(m_pTransformCom);
	m_Particles.clear();
}
