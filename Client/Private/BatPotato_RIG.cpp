#include "BatPotato_RIG.h"
#include "GameInstance.h"
#include "StateContext.h"
#include "BatPotato_RIGBullet.h"
_uint BatPotato_RIG::BatPotato_RIG_Id = 0;

BatPotato_RIG::BatPotato_RIG(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject(pDevice, pContext)
{
}

BatPotato_RIG::BatPotato_RIG(const BatPotato_RIG& rhs)
	: CGameObject(rhs)
{
}

HRESULT BatPotato_RIG::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT BatPotato_RIG::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Add_Components()))
		return E_FAIL;

	++BatPotato_RIG_Id;
	m_tInfo.wstrName = TO_WSTR("BatPotato_RIG" + to_string(BatPotato_RIG_Id));
	m_tInfo.wstrKey = ProtoTag();
	m_tInfo.ID = BatPotato_RIG_Id;

	tagBatPotato_RIGDesc tCloneDesc;
	if (nullptr != pArg)
		tCloneDesc = *(tagBatPotato_RIGDesc*)pArg;
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMLoadFloat4(&tCloneDesc.vPosition));

	// 상태 초기화
	m_pStateContextCom->TransitionTo(L"BatPotato_RIGIdle");
	
	// 노티파이 초기화. 활성화시키고, 준비한다음에 쏨.
	m_pModelCom->Add_TimeLineEvent("BatPotato_Attack01", L"Attack01", TIMELINE_EVENT(40.f, [this]() {
		static_cast<BatPotato_RIGBullet*>(m_pBullet)->Ready(m_pTransformCom->Get_State(CTransform::STATE_LOOK)
			, m_pTransformCom->Get_State(CTransform::STATE_POSITION));
		}), LOWER);

	return S_OK;
}

void BatPotato_RIG::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);

	m_pModelCom->Play_Animation(TimeDelta);
	if (nullptr != m_pStateContextCom)
		m_pStateContextCom->Tick(TimeDelta);

	if (m_pHealthCom->isZeroHP())
	{
		m_pStateContextCom->TransitionTo(L"BatPotato_RIGDeath");
		return;
	}

	if(nullptr != m_pColliderCom)
		m_pColliderCom->Tick(m_pTransformCom->Get_WorldMatrix());

	m_pBullet->Tick(TimeDelta);

	if (nullptr != m_pRaycastCom)
	{
		m_pRaycastCom->Tick(m_pTransformCom->Get_State(CTransform::STATE_POSITION),
			m_pTransformCom->Get_State(CTransform::STATE_LOOK));
	}
}

void BatPotato_RIG::Late_Tick(_double TimeDelta)
{
	__super::Late_Tick(TimeDelta);

	if (Facade->isRender(m_pRendererCom, m_pTransformCom))
	{
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this);

		// Dead상태가 아니면 콜라이더 넣음
		if (0 != lstrcmp(L"BatPotato_RIGDeath", m_pStateContextCom->GetCurState()))
			m_pColliderCom->Add_ColliderGroup(COLL_GROUP::MONSTER_BODY);
	}

	m_pBullet->Late_Tick(TimeDelta);

#ifdef _DEBUG
	if (nullptr != m_pColliderCom)
		m_pRendererCom->Add_DebugGroup(m_pColliderCom);
	 m_pRendererCom->Add_DebugGroup(m_pRaycastCom);
#endif
}

HRESULT BatPotato_RIG::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	if (FAILED(SetUp_ShaderResources()))
		return E_FAIL;

	_uint		iNumMeshes = m_pModelCom->Get_NumMeshes();

	for (size_t i = 0; i < iNumMeshes; i++)
	{
		m_pModelCom->Bind_BoneMatrices(m_pShaderCom, "g_BoneMatrices", i);

		m_pModelCom->Bind_Material(m_pShaderCom, "g_DiffuseTexture", i, TextureType_DIFFUSE);

		m_pShaderCom->Begin(0);

		m_pModelCom->Render(i);
	}
}

void BatPotato_RIG::OnCollision(CCollider::COLLISION_INFO tCollisionInfo, _double TimeDelta)
{
	// 총알은 레이어가 없다..
	if (dynamic_cast<Bullet*>(tCollisionInfo.pOtherGameObject))
	{
		m_pStateContextCom->TransitionTo(TEXT("BatPotato_RIGIdle"));
		m_pStateContextCom->TransitionTo(TEXT("BatPotato_RIGHit"));
	}

	// 바디콜라이더 끼리 부딪친 경우.
	if (tCollisionInfo.MyCollName == TEXT("Com_BodyColl")
		&& tCollisionInfo.OtherCollName == TEXT("Com_BodyColl")
		&& tCollisionInfo.OtherGameObjectLayerName == TEXT("Layer_Monster"))
	{
		tCollisionInfo.vOverLapVector.y = 0.f;
		m_pTransformCom->Go_Direction(TimeDelta, -XMLoadFloat3(&tCollisionInfo.vOverLapVector)
			, XMVectorGetX(XMVector3Length(XMLoadFloat3(&tCollisionInfo.vOverLapVector))));
	}

	m_pStateContextCom->OnCollision(tCollisionInfo, TimeDelta);
}

HRESULT BatPotato_RIG::Add_Components()
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
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_STATIC, L"Prototype_Component_Shader_VtxAnimMesh", L"Com_Shader", (CComponent**)&m_pShaderCom, &tShaderDesc), E_FAIL);

	CModel::CMODEL_DESC tModelDesc; tModelDesc.pOwner = this;
	if (0 == rand() % 2)
	{
		FAILED_CHECK_RETURN(__super::Add_Component(eLevelID, L"Prototype_Component_Model_BatPotato_RIG1", L"Com_Model", (CComponent**)&m_pModelCom, &tModelDesc), E_FAIL);
	}
	else
	{
		FAILED_CHECK_RETURN(__super::Add_Component(eLevelID, L"Prototype_Component_Model_BatPotato_RIG2", L"Com_Model", (CComponent**)&m_pModelCom, &tModelDesc), E_FAIL);
	}

	Raycast::RAYCAST_DESC tRaycastDesc;
	tRaycastDesc.pOwner = this;
	tRaycastDesc.vCenter = _float3(0.0f, 1.f, 0.f);
	tRaycastDesc.fLength = { 6.f };
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_STATIC, Raycast::ProtoTag(), L"Com_RayDetect", (CComponent**)&m_pRaycastCom, &tRaycastDesc), E_FAIL);

	CColliderAABB::CCOLLIDER_AABB_DESC tColliderAABBDesc;
	tColliderAABBDesc.pOwner = this;
	tColliderAABBDesc.Extents = _float3(0.3f, 1.f, 0.3f);
	tColliderAABBDesc.vCenter = _float3(0.f, tColliderAABBDesc.Extents.y, 0.f);
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_STATIC, CColliderAABB::ProtoTag(), L"Com_BodyColl", (CComponent**)&m_pColliderCom, &tColliderAABBDesc), E_FAIL);

	CNavigation::CNAVIGATION_DESC tNavigationdesc; tNavigationdesc.pOwner = this;
	tNavigationdesc.iCurrentIndex = { 0 };
	FAILED_CHECK_RETURN(__super::Add_Component(eLevelID, CNavigation::ProtoTag(), L"Com_Navigation", (CComponent**)&m_pNavigationCom, &tNavigationdesc), E_FAIL);

	BatPotatoRIGState::STATE_CONTEXT_DESC tStateContextDesc;
	tStateContextDesc.pOwner = this;
	FAILED_CHECK_RETURN(__super::Add_Component(eLevelID, TEXT("Prototype_Component_BatPotatoRIGState"), L"Com_BatPotatoRIGState", (CComponent**)&m_pStateContextCom, &tStateContextDesc), E_FAIL);

	Health::HEALTH_DESC tHealthDesc; 
	tHealthDesc.pOwner = this;
	tHealthDesc.iMaxHp = 1200;
	FAILED_CHECK_RETURN(__super::Add_Component(eLevelID, Health::ProtoTag(), L"Com_Health", (CComponent**)&m_pHealthCom, &tHealthDesc), E_FAIL);

	BatPotato_RIGBullet::tagBatPotato_RIGBulletDesc tBulletDesc;
	tBulletDesc.fDamage = 5.f;
	tBulletDesc.pOwner = this;
	tBulletDesc.vLook = m_pTransformCom->Get_State(CTransform::STATE_LOOK);
	FAILED_CHECK_RETURN(__super::Add_Composite(BatPotato_RIGBullet::ProtoTag(), L"Com_Bullet", (CComponent**)&m_pBullet, &tBulletDesc), E_FAIL);
	Safe_Release(pGameInstance);
	return S_OK;
}

HRESULT BatPotato_RIG::SetUp_ShaderResources()
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

BatPotato_RIG* BatPotato_RIG::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	BatPotato_RIG* pInstance = new BatPotato_RIG(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created BatPotato_RIG");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* BatPotato_RIG::Clone(void* pArg)
{
	BatPotato_RIG* pInstance = new BatPotato_RIG(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned BatPotato_RIG");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void BatPotato_RIG::Free(void)
{
	__super::Free();

	--BatPotato_RIG_Id;
	
	Safe_Release(m_pNavigationCom);
	Safe_Release(m_pColliderCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pStateContextCom);
	Safe_Release(m_pHealthCom);
	Safe_Release(m_pRaycastCom);
	Safe_Release(m_pBullet);

	/* Don't Forget Release for the VIBuffer or Model Component*/
}

