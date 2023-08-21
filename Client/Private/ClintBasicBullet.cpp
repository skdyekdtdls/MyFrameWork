#include "ClintBasicBullet.h"
#include "GameInstance.h"
#include "Effect4x4.h"
#include "CStone_Effect.h"
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
	//m_test->Reset_Effects();
	m_pEffect4x4Com->Reset();
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

	m_pEffect4x4Com->Tick(TimeDelta, m_pTransformCom->Get_State(CTransform::STATE_POSITION));
	//m_test->Tick(TimeDelta);
}

void ClintBasicBullet::Late_Tick(_double TimeDelta)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	m_pEffect4x4Com->Late_Tick(TimeDelta);
	__super::Late_Tick(TimeDelta);
	//m_test->Late_Tick(TimeDelta);
#ifdef _DEBUG
	m_pRendererCom->Add_DebugGroup(m_pColliderCom);
#endif
	
	Safe_Release(pGameInstance);
}

HRESULT ClintBasicBullet::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;
}

void ClintBasicBullet::ResetPool(void* pArg)
{	
	m_bDead = false;
	m_pTransformCom->Set_State(CTransform::STATE_LOOK, ((CLINT_BASIC_BULLET_DESC*)pArg)->vLook);
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMLoadFloat4(&((CLINT_BASIC_BULLET_DESC*)pArg)->vPosition));
	m_pTimeCounterCom->Enable();
	m_pEffect4x4Com->Reset();
	//m_test->Reset_Effects();
}

void ClintBasicBullet::SetDead()
{
	__super::SetDead();
	ObjectPool<ClintBasicBullet>::GetInstance()->PushPool(this);
	m_pTimeCounterCom->Reset();
	m_pTimeCounterCom->Disable();
	m_pEffect4x4Com->Disable();
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

	Effect4x4::EFFECT4X4_DESC tEffectDesc;
	tEffectDesc.pOwner = this;
	tEffectDesc.pTextureTag = TEXT("Prototype_Component_Texture_T_ky_flare09_4x4");
	FAILED_CHECK_RETURN(__super::Add_Composite(Effect4x4::ProtoTag(), L"Com_Effect4x4", (CComponent**)&m_pEffect4x4Com, &tEffectDesc), E_FAIL)

	CStone_Effect::STONE_EFFECT_DESC tStoneDesc;
	tStoneDesc.pOwner = this;
	tStoneDesc.iNumParticles = 30;
	FAILED_CHECK_RETURN(__super::Add_Composite(CStone_Effect::ProtoTag(), L"Com_Test", (CComponent**)&m_test, &tStoneDesc), E_FAIL);
	Safe_Release(pGameInstance);
	return S_OK;
}

void ClintBasicBullet::OnCollision(CCollider::COLLISION_INFO tCollisionInfo, _double TimeDelta)
{
	if (__super::isMonsterLayer(tCollisionInfo.OtherGameObjectLayerName))
	{
		__super::Damage(tCollisionInfo.pOtherGameObject);
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
	Safe_Release(m_pEffect4x4Com);
	Safe_Release(m_test);
}