#include "ClintBasicBullet.h"
#include "GameInstance.h"
#include "Effect_Atlas.h"
#include "CStone_Effect.h"
#include "PropelEffect.h"
#include "HitEffect.h"
#include "HitParticle.h"
#include "SoundMgr.h"
_uint ClintBasicBullet::ClintBasicBullet_Id = 0;

/* Don't Forget Release for the VIBuffer or Model Component*/

ClintBasicBullet::ClintBasicBullet(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: Bullet(pDevice, pContext)
{
}

ClintBasicBullet::ClintBasicBullet(const ClintBasicBullet& rhs)
	: Bullet(rhs)
{
}

HRESULT ClintBasicBullet::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT ClintBasicBullet::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Add_Components()))
		return E_FAIL;

	++ClintBasicBullet_Id;
	m_tInfo.wstrName = TO_WSTR("ClintBasicBullet" + to_string(ClintBasicBullet_Id));
	m_tInfo.wstrKey = ProtoTag();
	m_tInfo.ID = ClintBasicBullet_Id;

	CLINT_BASIC_BULLET_DESC tClintBasicBulletDesc;
	if (nullptr != pArg)
		tClintBasicBulletDesc = *(CLINT_BASIC_BULLET_DESC*)pArg;
	m_pTransformCom->Set_State(CTransform::STATE_LOOK, tClintBasicBulletDesc.vLook);
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMLoadFloat4(&tClintBasicBulletDesc.vPosition));
	
	m_pTimeCounterCom->Enable();
	m_pEffectAtlasCom->Reset();

	m_fRadian = DegreeBetweenVectors(XMVectorSet(0.f, 0.f, 1.f, 0.f), m_pTransformCom->Get_State(CTransform::STATE_LOOK));
	m_fRadian = XMConvertToRadians(m_fRadian);
	return S_OK;
}

void ClintBasicBullet::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);
	m_pTimeCounterCom->Tick(TimeDelta);

	// ¼ö¸í
	if (m_pTimeCounterCom->isGreaterThan(1.0))
	{
		SetDead();
	}

	m_pTransformCom->Go_Straight(TimeDelta);

	if (nullptr != m_pColliderCom)
	{
		m_pColliderCom->Tick(m_pTransformCom->Get_WorldMatrix());
		m_pColliderCom->Add_ColliderGroup(COLL_GROUP::PLAYER_BULLET);
	}

	m_fRadian = DegreeBetweenVectors(XMVectorSet(0.f, 0.f, 1.f, 0.f), m_pTransformCom->Get_State(CTransform::STATE_LOOK));
	m_fRadian = XMConvertToRadians(m_fRadian);
	m_pEffectAtlasCom->Tick(TimeDelta, m_pTransformCom->Get_State(CTransform::STATE_POSITION));

	if (nullptr != m_pPropelEffect)
		m_pPropelEffect->Tick(TimeDelta);
}

void ClintBasicBullet::Late_Tick(_double TimeDelta)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	m_pEffectAtlasCom->Late_Tick(TimeDelta);
	__super::Late_Tick(TimeDelta);
	//m_test->Late_Tick(TimeDelta);
	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONLIGHT, this);
#ifdef _DEBUG
	m_pRendererCom->Add_DebugGroup(m_pColliderCom);
#endif
	
	if (nullptr != m_pPropelEffect)
		m_pPropelEffect->Late_Tick(TimeDelta);

	Safe_Release(pGameInstance);
}

HRESULT ClintBasicBullet::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	if (FAILED(SetUp_ShaderResources()))
		return E_FAIL;

	m_pShaderCom->Begin(0);
	//m_pBufferCom->Render();
}

void ClintBasicBullet::ResetPool(void* pArg)
{	
	m_bDead = false;
	m_pTransformCom->Set_State(CTransform::STATE_LOOK, ((CLINT_BASIC_BULLET_DESC*)pArg)->vLook);
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMLoadFloat4(&((CLINT_BASIC_BULLET_DESC*)pArg)->vPosition));
	m_pTimeCounterCom->Enable();
	m_pPropelEffect->Reset_Effects();
	m_pEffectAtlasCom->Reset();
	//m_test->Reset_Effects();
}

void ClintBasicBullet::SetDead()
{
	__super::SetDead();
	ObjectPool<ClintBasicBullet>::GetInstance()->PushPool(this);
	m_pTimeCounterCom->Reset();
	m_pTimeCounterCom->Disable();
	//m_pEffectAtlasCom->Disable();
}

HRESULT ClintBasicBullet::Add_Components()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);
	LEVELID eLevelID = static_cast<LEVELID>(pGameInstance->Get_NextLevelIndex());

	CRenderer::CRENDERER_DESC tRendererDesc; tRendererDesc.pOwner = this;
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_STATIC, CRenderer::ProtoTag(), L"Com_Renderer", (CComponent**)&m_pRendererCom, &tRendererDesc), E_FAIL);

	CTransform::CTRANSFORM_DESC TransformDesc{ 20.0, XMConvertToRadians(720.f) }; TransformDesc.pOwner = this;
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_STATIC, CTransform::ProtoTag(), L"Com_Transform", (CComponent**)&m_pTransformCom
		, &TransformDesc), E_FAIL);

	CColliderSphere::CCOLLIDER_SPHERE_DESC tColliderSphereDesc;
	tColliderSphereDesc.pOwner = this;
	tColliderSphereDesc.fRadius = { 0.2f };
	tColliderSphereDesc.vCenter = _float3(0.0f, 0.f, 0.f);
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_STATIC, CColliderSphere::ProtoTag(), L"Com_BodyColl", (CComponent**)&m_pColliderCom, &tColliderSphereDesc), E_FAIL);

	TimeCounter::TIME_COUNTER_DESC tTimeCounterDesc;
	tTimeCounterDesc.pOwner = this;
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_STATIC, TimeCounter::ProtoTag(), L"Com_TimeCounter", (CComponent**)&m_pTimeCounterCom, &tTimeCounterDesc), E_FAIL);

	CTexture::tagCTextureDesc tTextureDesc;
	tTextureDesc.pOwner = this;
	FAILED_CHECK_RETURN(__super::Add_Component(eLevelID, L"Prototype_Component_Texture_Bullet01", L"Com_Texture", (CComponent**)&m_pTextureCom, &tTextureDesc), E_FAIL);

	CShader::CSHADER_DESC tShaderDesc;
	tShaderDesc.pOwner = this;
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_STATIC, L"Prototype_Component_Shader_VtxBullet01", L"Com_Shader", (CComponent**)&m_pShaderCom, &tShaderDesc), E_FAIL);
	
	CVIBuffer_Point_Instance::tagCVIBuffer_Point_InstanceDesc tBufferDesc;
	tBufferDesc.pOwner = this;
	tBufferDesc.iNumInstance = 1;
	FAILED_CHECK_RETURN(Add_Component(eLevelID, CVIBuffer_Point_Instance::ProtoTag(), L"Com_Buffer"
		, (CComponent**)&m_pBufferCom, &tBufferDesc), E_FAIL);
	Effect_Atlas::EFFECT_ATLAS_DESC tEffectDesc;
	tEffectDesc.pOwner = this;
	tEffectDesc.iRow = 4;
	tEffectDesc.iCol = 4;
	tEffectDesc.pTextureTag = TEXT("Prototype_Component_Texture_T_ky_flare09_4x4");
	FAILED_CHECK_RETURN(__super::Add_Composite(Effect_Atlas::ProtoTag(), L"Com_Effect4x4", (CComponent**)&m_pEffectAtlasCom, &tEffectDesc), E_FAIL)
	//
	//CStone_Effect::STONE_EFFECT_DESC tStoneDesc;
	//tStoneDesc.pOwner = this;
	//tStoneDesc.iNumParticles = 30;
	//FAILED_CHECK_RETURN(__super::Add_Composite(CStone_Effect::ProtoTag(), L"Com_Test", (CComponent**)&m_test, &tStoneDesc), E_FAIL);
	
	PropelEffect::tagPropelEffectDesc tPropelEffectDesc;
	tPropelEffectDesc.iNumParticles = 20;
	tPropelEffectDesc.pOwner = this;
	tPropelEffectDesc.pTextureProtoTag = L"Prototype_Component_Texture_Snow";
	FAILED_CHECK_RETURN(__super::Add_Composite(PropelEffect::ProtoTag(), L"Com_PropelEffect", (CComponent**)&m_pPropelEffect, &tPropelEffectDesc), E_FAIL);
		
	Safe_Release(pGameInstance);
	return S_OK;
}

HRESULT ClintBasicBullet::SetUp_ShaderResources()
{
	_float4x4 MyMatrix = m_pTransformCom->Get_WorldFloat4x4();
	FAILED_CHECK_RETURN(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &MyMatrix), E_FAIL);

	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	MyMatrix = pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_VIEW);
	FAILED_CHECK_RETURN(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &MyMatrix), E_FAIL);

	MyMatrix = pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_PROJ);
	FAILED_CHECK_RETURN(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &MyMatrix), E_FAIL);

	FAILED_CHECK_RETURN(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture"), E_FAIL);

	_float4 vCamPos = pGameInstance->Get_CamPositionFloat4();
	FAILED_CHECK_RETURN(m_pShaderCom->Bind_RawValue("g_vCamPosition", &vCamPos, sizeof(_float4)), E_FAIL);
	FAILED_CHECK_RETURN(m_pShaderCom->Bind_RawValue("g_fRadian", &m_fRadian, sizeof(float)), E_FAIL);
	
	Safe_Release(pGameInstance);

	return S_OK;
}

void ClintBasicBullet::OnCollision(CCollider::COLLISION_INFO tCollisionInfo, _double TimeDelta)
{
	if (__super::isMonsterLayer(tCollisionInfo.OtherGameObjectLayerName))
	{
		__super::Damage(tCollisionInfo.pOtherGameObject);

		CGameInstance* pGameInstance = CGameInstance::GetInstance();
		Safe_AddRef(pGameInstance);

		//HitEffect::tagHitEffectDesc Desc;
		//Desc.iCol = 8;
		//Desc.iRow = 8;
		//Desc.pTextureTag = L"Prototype_Component_Texture_Explodebase1_2k";
		//XMStoreFloat4(&Desc.vPosition, m_pTransformCom->Get_State(CTransform::STATE_POSITION));
		//HitEffect* pEffectAtlas = nullptr;
		//pEffectAtlas = static_cast<HitEffect*>(pGameInstance->Add_GameObject(pGameInstance->Get_CurLevelIndex(), HitEffect::ProtoTag()
		//	, L"Layer_Effect", &Desc));
		//NULL_CHECK(pEffectAtlas);
		//pEffectAtlas->Reset();

		HitParticle::tagHitParticleDesc tParticleDesc;
		tParticleDesc.iNumParticles = 20;
		XMStoreFloat4(&tParticleDesc.vPosition, m_pTransformCom->Get_State(CTransform::STATE_POSITION));
		NULL_CHECK(pGameInstance->Add_GameObject(pGameInstance->Get_CurLevelIndex(), HitParticle::ProtoTag()
			, L"Layer_Effect", &tParticleDesc));

		SoundMgr->StopSound(CHANNELID::BULLET_HIT);
		SoundMgr->PlaySound(L"BulletHit.mp3", CHANNELID::BULLET_HIT, 0.7f);

		Safe_Release(pGameInstance);
		SetDead();
	}
}

ClintBasicBullet* ClintBasicBullet::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	ClintBasicBullet* pInstance = new ClintBasicBullet(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created ClintBasicBullet");
		Safe_Release(pInstance);
	}
	return pInstance;
}

Bullet* ClintBasicBullet::Clone(void* pArg)
{
	ClintBasicBullet* pInstance = new ClintBasicBullet(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned ClintBasicBullet");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void ClintBasicBullet::Free(void)
{
	__super::Free();

	--ClintBasicBullet_Id;
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pColliderCom);
	Safe_Release(m_pTimeCounterCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pBufferCom);
	Safe_Release(m_pEffectAtlasCom);
	Safe_Release(m_pPropelEffect);
	//Safe_Release(m_test);
}