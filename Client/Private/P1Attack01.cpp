#include "P1Attack01.h"
#include "GameInstance.h"
#include "Effect_Atlas.h"
_uint P1Attack01::P1Attack01_Id = 0;

/* Don't Forget Release for the VIBuffer or Model Component*/

P1Attack01::P1Attack01(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: Bullet(pDevice, pContext)
{
}

P1Attack01::P1Attack01(const P1Attack01& rhs)
	: Bullet(rhs)
{
}

HRESULT P1Attack01::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}


HRESULT P1Attack01::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Add_Components()))
		return E_FAIL;

	++P1Attack01_Id;
	m_tInfo.wstrName = TO_WSTR("P1Attack01" + to_string(P1Attack01_Id));
	m_tInfo.wstrKey = ProtoTag();
	m_tInfo.ID = P1Attack01_Id;

	tagP1Attack01Desc tP1Attack01Desc;
	if (nullptr != pArg)
		tP1Attack01Desc = *(tagP1Attack01Desc*)pArg;
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMLoadFloat4(&tP1Attack01Desc.vPosition));

	m_pTimeCounterCom->Enable();
	m_pEffectAtlas->Play_Loop(0, 8);

	return S_OK;
}

void P1Attack01::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);
	m_pTimeCounterCom->Tick(TimeDelta);

	// 수명
	if (m_pTimeCounterCom->isEuqalWith(3.0))
		__super::SetDead();

	m_pTransformCom->Go_Straight(TimeDelta);

	if (nullptr != m_pColliderCom)
	{
		m_pColliderCom->Tick(m_pTransformCom->Get_WorldMatrix());
		m_pColliderCom->Add_ColliderGroup(COLL_GROUP::MONSTER_BULLET);
	}
	m_pEffectAtlas->Tick(TimeDelta, m_pTransformCom->Get_State(CTransform::STATE_POSITION));
	// 	m_pModelCom->Play_Animation(TimeDelta);
}

void P1Attack01::Late_Tick(_double TimeDelta)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);
	m_pEffectAtlas->Late_Tick(TimeDelta);
	__super::Late_Tick(TimeDelta);
#ifdef _DEBUG
	m_pRendererCom->Add_DebugGroup(m_pColliderCom);
#endif
	Safe_Release(pGameInstance);
}

HRESULT P1Attack01::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	//_uint		iNumMeshes = m_pModelCom->Get_NumMeshes();

	//for (size_t i = 0; i < iNumMeshes; i++)
	//{
	//	m_pModelCom->Bind_BoneMatrices(m_pShaderCom, "g_BoneMatrices", i);

	//	m_pModelCom->Bind_Material(m_pShaderCom, "g_DiffuseTexture", i, TextureType_DIFFUSE);
	//	

	//	m_pShaderCom->Begin(0);

	//	m_pModelCom->Render(i);
	//}

	// 만약에 모델 컴포넌트 안쓰면 이걸로 쓰면된다.
	// m_pShaderCom->Begin(0);

#ifdef _DEBUG

#endif
}

void P1Attack01::ResetPool(void* pArg)
{
	m_bDead = false;
	_vector vPosition = XMLoadFloat4(&((tagP1Attack01Desc*)pArg)->vPosition);
	_vector vTargetPos = ((tagP1Attack01Desc*)pArg)->vTargetPos;

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPosition);
	m_pTransformCom->LookAt(vTargetPos);
	m_pTimeCounterCom->Enable();
}

void P1Attack01::SetDead()
{
	__super::SetDead();
	ObjectPool<P1Attack01>::GetInstance()->PushPool(this);

	m_pTimeCounterCom->Reset();
	m_pTimeCounterCom->Disable();
}

HRESULT P1Attack01::Add_Components()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);
	LEVELID eLevelID = static_cast<LEVELID>(pGameInstance->Get_NextLevelIndex());

	CRenderer::CRENDERER_DESC tRendererDesc; tRendererDesc.pOwner = this;
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_STATIC, CRenderer::ProtoTag(), L"Com_Renderer", (CComponent**)&m_pRendererCom, &tRendererDesc), E_FAIL);

	CTransform::CTRANSFORM_DESC TransformDesc{ 7.0, XMConvertToRadians(720.f) }; TransformDesc.pOwner = this;
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_STATIC, CTransform::ProtoTag(), L"Com_Transform", (CComponent**)&m_pTransformCom
		, &TransformDesc), E_FAIL);

	CColliderSphere::CCOLLIDER_SPHERE_DESC tColliderSphereDesc;
	tColliderSphereDesc.pOwner = this;
	tColliderSphereDesc.fRadius = { 1.f };
	tColliderSphereDesc.vCenter = _float3(0.0f, 0.f, 0.f);
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_STATIC, CColliderSphere::ProtoTag(), L"Com_BodyColl", (CComponent**)&m_pColliderCom, &tColliderSphereDesc), E_FAIL);

	TimeCounter::TIME_COUNTER_DESC tTimeCounterDesc;
	tTimeCounterDesc.pOwner = this;
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_STATIC, TimeCounter::ProtoTag(), L"Com_TimeCounter", (CComponent**)&m_pTimeCounterCom, &tTimeCounterDesc), E_FAIL);

	Effect_Atlas::EFFECT_ATLAS_DESC tEffectAtlasDesc;
	tEffectAtlasDesc.iCol = 4;
	tEffectAtlasDesc.iRow = 4;
	tEffectAtlasDesc.pOwner = this;
	tEffectAtlasDesc.pTextureTag = TEXT("Prototype_Component_Texture_T_ky_flare09_4x4");
	FAILED_CHECK_RETURN(__super::Add_Composite(Effect_Atlas::ProtoTag()
		, L"Com_Effect", (CComponent**)&m_pEffectAtlas, &tEffectAtlasDesc), E_FAIL);
	
	Safe_Release(pGameInstance);
	return S_OK;
}



void P1Attack01::OnCollision(CCollider::COLLISION_INFO tCollisionInfo, _double TimeDelta)
{
	if (0 == strcmp(tCollisionInfo.OtherGameObejctName.c_str(), "Clint1"))
	{
		__super::Damage(tCollisionInfo.pOtherGameObject);
		SetDead();
	}
}

P1Attack01* P1Attack01::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	P1Attack01* pInstance = new P1Attack01(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created P1Attack01");
		Safe_Release(pInstance);
	}
	return pInstance;
}

Bullet* P1Attack01::Clone(void* pArg)
{
	P1Attack01* pInstance = new P1Attack01(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned P1Attack01");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void P1Attack01::Free(void)
{
	__super::Free();

	--P1Attack01_Id;
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pColliderCom);
	Safe_Release(m_pTimeCounterCom);
	Safe_Release(m_pEffectAtlas);
}
