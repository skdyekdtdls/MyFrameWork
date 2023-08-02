#include "Spider.h"
#include "GameInstance.h"
#include "StateContext.h"
#include "SpiderBullet.h"
#include "MonsterHP.h"
_uint Spider::Spider_Id = 0;

Spider::Spider(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject(pDevice, pContext)
{
}

Spider::Spider(const Spider& rhs)
	: CGameObject(rhs)
{
}

HRESULT Spider::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT Spider::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Add_Components()))
		return E_FAIL;

	// 태그초기화
	++Spider_Id;
	m_tInfo.wstrName = TO_WSTR("Spider" + to_string(Spider_Id));
	m_tInfo.wstrKey = ProtoTag();
	m_tInfo.ID = Spider_Id;

	// 상태 초기화
	m_pStateContextCom->TransitionTo(L"SpiderAppear");

	// Desc초기화
	tagSpiderDesc tCloneDesc;
	if (nullptr != pArg)
		tCloneDesc = *(tagSpiderDesc*)pArg;
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMLoadFloat4(&tCloneDesc.vPosition));

	// 노티파이 초기화
	SetUp_Notify();

	// 옵저버 이벤트 추가
	m_pMonsterHP->GetObserver()->Subscribe(L"isZeroHP", [this]() {
		if (m_pMonsterHP->isZeroHP())
		{
			m_pStateContextCom->TransitionTo(L"SpiderDead");
		}
		});
	m_pMonsterHP->Disable();

	return S_OK;
}

void Spider::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);

	m_pModelCom->Play_Animation(TimeDelta);

	if (nullptr != m_pStateContextCom)
		m_pStateContextCom->Tick(TimeDelta);

	m_pMonsterHP->Tick(TimeDelta);

	if (nullptr != m_pColliderCom)
		m_pColliderCom->Tick(m_pTransformCom->Get_WorldMatrix());

	if (nullptr != m_pRaycastCom)
	{
		m_pRaycastCom->Tick(m_pTransformCom->Get_State(CTransform::STATE_POSITION),
			m_pTransformCom->Get_State(CTransform::STATE_LOOK));
	}
}

void Spider::Late_Tick(_double TimeDelta)
{
	__super::Late_Tick(TimeDelta);
	m_pMonsterHP->Late_Tick(TimeDelta);
	if (Single->isRender(m_pRendererCom, m_pTransformCom))
	{
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this);

		// Dead상태가 아니면 콜라이더 넣음
		if (0 != lstrcmp(L"SpiderDead", m_pStateContextCom->GetCurState()))
			m_pColliderCom->Add_ColliderGroup(COLL_GROUP::MONSTER_BODY);
	}

#ifdef _DEBUG
	if (nullptr != m_pColliderCom)
		m_pRendererCom->Add_DebugGroup(m_pColliderCom);
	m_pRendererCom->Add_DebugGroup(m_pRaycastCom);
#endif
}

HRESULT Spider::Render()
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

void Spider::OnCollision(CCollider::COLLISION_INFO tCollisionInfo, _double TimeDelta)
{
	// 총알은 레이어가 없다..
	if (dynamic_cast<Bullet*>(tCollisionInfo.pOtherGameObject) && false == m_pMonsterHP->isZeroHP())
	{
		m_pStateContextCom->TransitionTo(TEXT("SpiderIdle"));
		m_pStateContextCom->TransitionTo(TEXT("SpiderHit"));
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

HRESULT Spider::SetUp_Notify()
{

	m_pModelCom->Add_TimeLineEvent("spider_attack", L"SpiderAttack", TIMELINE_EVENT(25.0, [this]() {
		CGameInstance* pGameInstance = CGameInstance::GetInstance();
		Safe_AddRef(pGameInstance);
		// TODO
		SpiderBullet::tagSpiderBulletDesc tBulletDesc;
		_vector vPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
		_vector vLook = m_pTransformCom->Get_State(CTransform::STATE_LOOK);

		tBulletDesc.pOwner = this;
		tBulletDesc.fDamage = 10.f;
		vPosition += vLook * 1.f;
		XMStoreFloat4(&tBulletDesc.vPosition, vPosition);

		Bullet* pBullet = ObjectPool<SpiderBullet>::GetInstance()->PopPool(SpiderBullet::ProtoTag(), &tBulletDesc);

		pGameInstance->AddToLayer(pGameInstance->Get_CurLevelIndex(), L"Layer_Bullet", pBullet);

		Safe_Release(pGameInstance);
		}));

	m_pModelCom->Add_TimeLineEvent("spider_attack_b", L"SpiderAttack", TIMELINE_EVENT(25.0, [this]() {
		CGameInstance* pGameInstance = CGameInstance::GetInstance();
		Safe_AddRef(pGameInstance);
		// TODO
		SpiderBullet::tagSpiderBulletDesc tBulletDesc;
		_vector vPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
		_vector vLook = m_pTransformCom->Get_State(CTransform::STATE_LOOK);

		tBulletDesc.pOwner = this;
		tBulletDesc.fDamage = 10.f;
		vPosition += vLook * 1.f;
		XMStoreFloat4(&tBulletDesc.vPosition, vPosition);

		Bullet* pBullet = ObjectPool<SpiderBullet>::GetInstance()->PopPool(SpiderBullet::ProtoTag(), &tBulletDesc);

		pGameInstance->AddToLayer(pGameInstance->Get_CurLevelIndex(), L"Layer_Bullet", pBullet);

		Safe_Release(pGameInstance);

		}));

	m_pModelCom->Add_TimeLineEvent("spider_attack_mid", L"SpiderAttack", TIMELINE_EVENT(25.0, [this]() {
		CGameInstance* pGameInstance = CGameInstance::GetInstance();
		Safe_AddRef(pGameInstance);
		// TODO
		SpiderBullet::tagSpiderBulletDesc tBulletDesc;
		_vector vPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
		_vector vLook = m_pTransformCom->Get_State(CTransform::STATE_LOOK);

		tBulletDesc.pOwner = this;
		tBulletDesc.fDamage = 10.f;
		vPosition += vLook * 1.f;
		XMStoreFloat4(&tBulletDesc.vPosition, vPosition);

		Bullet* pBullet = ObjectPool<SpiderBullet>::GetInstance()->PopPool(SpiderBullet::ProtoTag(), &tBulletDesc);

		pGameInstance->AddToLayer(pGameInstance->Get_CurLevelIndex(), L"Layer_Bullet", pBullet);

		Safe_Release(pGameInstance);

		}));


	// spider_attack      75.0
	// spider_attack_b    75.0
	// spider_attack_mid  75.0



	return S_OK;
}

HRESULT Spider::Add_Components()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);
	LEVELID eLevelID = static_cast<LEVELID>(pGameInstance->Get_NextLevelIndex());

	CRenderer::CRENDERER_DESC tRendererDesc; tRendererDesc.pOwner = this;
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_STATIC, CRenderer::ProtoTag(), L"Com_Renderer", (CComponent**)&m_pRendererCom, &tRendererDesc), E_FAIL);

	CTransform::CTRANSFORM_DESC TransformDesc{ 4.0, XMConvertToRadians(360.f) }; TransformDesc.pOwner = this;
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_STATIC, CTransform::ProtoTag(), L"Com_Transform", (CComponent**)&m_pTransformCom
		, &TransformDesc), E_FAIL);

	CShader::CSHADER_DESC tShaderDesc; tShaderDesc.pOwner = this;
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_STATIC, L"Prototype_Component_Shader_VtxAnimMesh", L"Com_Shader", (CComponent**)&m_pShaderCom, &tShaderDesc), E_FAIL);

	//_uint iLogic = RandomIntFrom_A_To_B(0, 6);
	
	CModel::CMODEL_DESC tModelDesc; tModelDesc.pOwner = this;
	switch (6)
	{
	case 0:
		FAILED_CHECK_RETURN(__super::Add_Component(eLevelID, L"Prototype_Component_Model_MicroSpider1", L"Com_Model", (CComponent**)&m_pModelCom, &tModelDesc), E_FAIL);
		break;
	case 1:
		FAILED_CHECK_RETURN(__super::Add_Component(eLevelID, L"Prototype_Component_Model_MicroSpider2", L"Com_Model", (CComponent**)&m_pModelCom, &tModelDesc), E_FAIL);
		break;
	case 2:
		FAILED_CHECK_RETURN(__super::Add_Component(eLevelID, L"Prototype_Component_Model_Spider1", L"Com_Model", (CComponent**)&m_pModelCom, &tModelDesc), E_FAIL);
		break;
	case 3:
		FAILED_CHECK_RETURN(__super::Add_Component(eLevelID, L"Prototype_Component_Model_Spider2", L"Com_Model", (CComponent**)&m_pModelCom, &tModelDesc), E_FAIL);
		break;
	case 4:
		FAILED_CHECK_RETURN(__super::Add_Component(eLevelID, L"Prototype_Component_Model_Spider3", L"Com_Model", (CComponent**)&m_pModelCom, &tModelDesc), E_FAIL);
		break;
	case 5:
		FAILED_CHECK_RETURN(__super::Add_Component(eLevelID, L"Prototype_Component_Model_SummonSpider1", L"Com_Model", (CComponent**)&m_pModelCom, &tModelDesc), E_FAIL);
		break;
	case 6:
		FAILED_CHECK_RETURN(__super::Add_Component(eLevelID, L"Prototype_Component_Model_SummonSpider2", L"Com_Model", (CComponent**)&m_pModelCom, &tModelDesc), E_FAIL);
		break;
	}

	CColliderAABB::CCOLLIDER_AABB_DESC tColliderAABBDesc;
	tColliderAABBDesc.pOwner = this;
	tColliderAABBDesc.Extents = _float3(0.3f, 1.f, 0.3f);
	tColliderAABBDesc.vCenter = _float3(0.0f, tColliderAABBDesc.Extents.y, 0.f);
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_STATIC, CColliderAABB::ProtoTag(), L"Com_BodyColl", (CComponent**)&m_pColliderCom, &tColliderAABBDesc), E_FAIL);

	Raycast::RAYCAST_DESC tRaycastDesc;
	tRaycastDesc.pOwner = this;
	tRaycastDesc.vCenter = _float3(0.0f, 1.f, 0.f);
	tRaycastDesc.fLength = { 1.f };
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_STATIC, Raycast::ProtoTag(), L"Com_RayDetect", (CComponent**)&m_pRaycastCom, &tRaycastDesc), E_FAIL);

	CNavigation::CNAVIGATION_DESC tNavigationdesc; tNavigationdesc.pOwner = this;
	tNavigationdesc.iCurrentIndex = { 0 };
	FAILED_CHECK_RETURN(__super::Add_Component(eLevelID, CNavigation::ProtoTag(), L"Com_Navigation", (CComponent**)&m_pNavigationCom, &tNavigationdesc), E_FAIL);

	SpiderState::STATE_CONTEXT_DESC tStateContextDesc;
	tStateContextDesc.pOwner = this;
	FAILED_CHECK_RETURN(__super::Add_Component(eLevelID, TEXT("Prototype_Component_SpiderState"), L"Com_StateContext", (CComponent**)&m_pStateContextCom, &tStateContextDesc), E_FAIL);

	MonsterHP::tagMonsterHPDesc tMonsterHPDesc;
	tMonsterHPDesc.pOwner = this;
	tMonsterHPDesc.fSize = _float2(60, 15);
	FAILED_CHECK_RETURN(__super::Add_Composite(MonsterHP::ProtoTag(), L"Com_HP", (CComponent**)&m_pMonsterHP, &tMonsterHPDesc), E_FAIL);

	Safe_Release(pGameInstance);
	return S_OK;
}

HRESULT Spider::SetUp_ShaderResources()
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

Spider* Spider::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	Spider* pInstance = new Spider(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created Spider");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* Spider::Clone(void* pArg)
{
	Spider* pInstance = new Spider(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned Spider");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void Spider::Free(void)
{
	__super::Free();

	--Spider_Id;

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pColliderCom);
	Safe_Release(m_pStateContextCom);
	Safe_Release(m_pNavigationCom);
	Safe_Release(m_pRaycastCom);
	Safe_Release(m_pMonsterHP);
}
