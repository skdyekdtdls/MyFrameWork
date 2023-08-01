#include "Queen_Moggoth.h"
#include "GameInstance.h"
#include "StateContext.h"
#include "P1Attack01.h"
#include "P1Attack02.h"
#include "P1Attack03.h"
#include "P1Attack04.h"
#include "P2Attack01.h"
#include "P2Attack02.h"
#include "P2Attack03.h"
#include "P2Attack04.h"
_uint Queen_Moggoth::Queen_Moggoth_Id = 0;

Queen_Moggoth::Queen_Moggoth(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject(pDevice, pContext)
{
}

Queen_Moggoth::Queen_Moggoth(const Queen_Moggoth& rhs)
	: CGameObject(rhs)
{
}

HRESULT Queen_Moggoth::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT Queen_Moggoth::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Add_Components()))
		return E_FAIL;

	// 태그초기화
	++Queen_Moggoth_Id;
	m_tInfo.wstrName = TO_WSTR("Queen_Moggoth" + to_string(Queen_Moggoth_Id));
	m_tInfo.wstrKey = ProtoTag();
	m_tInfo.ID = Queen_Moggoth_Id;

	// 상태 초기화
	m_pStateContextCom->TransitionTo(L"Queen_MoggothAppear");

	// 옵저버
	m_pHealthCom->Subscribe(L"PhaseChange", [this]() {
		if (m_pHealthCom->HPPercent() < 50.f)
		{
			m_pStateContextCom->TransitionTo(L"Queen_Moggoth_P1_TO_P2");
			m_pHealthCom->UnSubscribeDelay(L"PhaseChange");
		}
		});

	// Desc초기화
	tagQueen_MoggothDesc tCloneDesc;
	if (nullptr != pArg)
		tCloneDesc = *(tagQueen_MoggothDesc*)pArg;
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMLoadFloat4(&tCloneDesc.vPosition));

	// 노티파이 초기화
	SetUp_Notify();

	return S_OK;
}

void Queen_Moggoth::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);

	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);
	static int asdf = 0;
	if (pGameInstance->Key_Down(DIK_P))
	{
		asdf++;
		m_pModelCom->Set_AnimByIndex(asdf);
	}
	Safe_Release(pGameInstance);

	m_pModelCom->Play_Animation(TimeDelta);

	if (nullptr != m_pStateContextCom)
		m_pStateContextCom->Tick(TimeDelta);

	if (m_pHealthCom->isZeroHP())
	{
		m_pStateContextCom->TransitionTo(L"Queen_MoggothDead");
		return;
	}

	if (nullptr != m_pColliderCom)
		m_pColliderCom->Tick(m_pTransformCom->Get_WorldMatrix());

	if (nullptr != m_pRaycastCom)
	{
		m_pRaycastCom->Tick(m_pTransformCom->Get_State(CTransform::STATE_POSITION),
			m_pTransformCom->Get_State(CTransform::STATE_LOOK));
	}
}

void Queen_Moggoth::Late_Tick(_double TimeDelta)
{
	__super::Late_Tick(TimeDelta);

	if (Single->isRender(m_pRendererCom, m_pTransformCom))
	{
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this);

		// Dead상태가 아니면 콜라이더 넣음
		if (0 != lstrcmp(L"Queen_MoggothDead", m_pStateContextCom->GetCurState()))
			m_pColliderCom->Add_ColliderGroup(COLL_GROUP::MONSTER_BODY);
	}

#ifdef _DEBUG
	if (nullptr != m_pColliderCom)
		m_pRendererCom->Add_DebugGroup(m_pColliderCom);
	m_pRendererCom->Add_DebugGroup(m_pRaycastCom);
#endif
}

HRESULT Queen_Moggoth::Render()
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

void Queen_Moggoth::OnCollision(CCollider::COLLISION_INFO tCollisionInfo, _double TimeDelta)
{
	m_pStateContextCom->OnCollision(tCollisionInfo, TimeDelta);
}

HRESULT Queen_Moggoth::Add_Components()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);
	LEVELID eLevelID = static_cast<LEVELID>(pGameInstance->Get_NextLevelIndex());

	CRenderer::CRENDERER_DESC tRendererDesc; tRendererDesc.pOwner = this;
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_STATIC, CRenderer::ProtoTag(), L"Com_Renderer", (CComponent**)&m_pRendererCom, &tRendererDesc), E_FAIL);

	CTransform::CTRANSFORM_DESC TransformDesc{ 3.5, XMConvertToRadians(90.f) }; TransformDesc.pOwner = this;
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_STATIC, CTransform::ProtoTag(), L"Com_Transform", (CComponent**)&m_pTransformCom
		, &TransformDesc), E_FAIL);

	CShader::CSHADER_DESC tShaderDesc; tShaderDesc.pOwner = this;
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_STATIC, L"Prototype_Component_Shader_VtxAnimMesh", L"Com_Shader", (CComponent**)&m_pShaderCom, &tShaderDesc), E_FAIL);

	CModel::CMODEL_DESC tModelDesc; tModelDesc.pOwner = this;
	FAILED_CHECK_RETURN(__super::Add_Component(eLevelID, L"Prototype_Component_Model_Queen_Moggoth", L"Com_Model", (CComponent**)&m_pModelCom, &tModelDesc), E_FAIL);

	CColliderAABB::CCOLLIDER_AABB_DESC tColliderAABBDesc;
	tColliderAABBDesc.pOwner = this;
	tColliderAABBDesc.Extents = _float3(1.f, 1.f, 1.f);
	tColliderAABBDesc.vCenter = _float3(0.f, tColliderAABBDesc.Extents.y, 0.f);
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_STATIC, CColliderAABB::ProtoTag(), L"Com_BodyColl", (CComponent**)&m_pColliderCom, &tColliderAABBDesc), E_FAIL);

	
	tColliderAABBDesc.pOwner = this;
	tColliderAABBDesc.Extents = _float3(2.f, 2.f, 2.f);
	tColliderAABBDesc.vCenter = _float3(0.f, tColliderAABBDesc.Extents.y, 0.f);
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_STATIC, CColliderAABB::ProtoTag(), L"Com_Attack04Coll", (CComponent**)&m_pAttack04CollCom, &tColliderAABBDesc), E_FAIL);

	Raycast::RAYCAST_DESC tRaycastDesc;
	tRaycastDesc.pOwner = this;
	tRaycastDesc.vCenter = _float3(0.0f, 1.f, 0.f);
	tRaycastDesc.fLength = { 6.f };
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_STATIC, Raycast::ProtoTag(), L"Com_RayDetect", (CComponent**)&m_pRaycastCom, &tRaycastDesc), E_FAIL);

	CNavigation::CNAVIGATION_DESC tNavigationdesc; tNavigationdesc.pOwner = this;
	tNavigationdesc.iCurrentIndex = { 0 };
	FAILED_CHECK_RETURN(__super::Add_Component(eLevelID, CNavigation::ProtoTag(), L"Com_Navigation", (CComponent**)&m_pNavigationCom, &tNavigationdesc), E_FAIL);

	Queen_MoggothState::STATE_CONTEXT_DESC tStateContextDesc;
	tStateContextDesc.pOwner = this;
	FAILED_CHECK_RETURN(__super::Add_Component(eLevelID, TEXT("Prototype_Component_Queen_MoggothState"), L"Com_StateContext", (CComponent**)&m_pStateContextCom, &tStateContextDesc), E_FAIL);

	Health::HEALTH_DESC tHealthDesc;
	tHealthDesc.pOwner = this;
	tHealthDesc.iMaxHp = 1000;
	FAILED_CHECK_RETURN(__super::Add_Component(eLevelID, Health::ProtoTag(), L"Com_Health", (CComponent**)&m_pHealthCom, &tHealthDesc), E_FAIL);

	Safe_Release(pGameInstance);
	return S_OK;
}

HRESULT Queen_Moggoth::SetUp_Notify()
{
	m_pModelCom->Add_TimeLineEvent("Queen_Moggoth_P1_Attack01", L"P1_Attack01", TIMELINE_EVENT(30.0, [this]() {
		CGameInstance* pGameInstance = CGameInstance::GetInstance();
		Safe_AddRef(pGameInstance);

		_vector vPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
		P1Attack01* pBullet;
		P1Attack01::tagP1Attack01Desc tResetDesc;

		vPosition.m128_f32[1] += 4.8f;

		tResetDesc.fDamage = 10.f;
		tResetDesc.pOwner = this;
		DirectX::XMStoreFloat4(&tResetDesc.vPosition, vPosition);
		tResetDesc.vTargetPos = Single->GetClintPosition();

		pBullet = ObjectPool<P1Attack01>::GetInstance()->PopPool(P1Attack01::ProtoTag(), &tResetDesc);
		pGameInstance->AddToLayer(pGameInstance->Get_CurLevelIndex(), L"Layer_Bullet", pBullet);

		Safe_Release(pGameInstance);

		}));

	m_pModelCom->Add_TimeLineEvent("Queen_Moggoth_P1_Attack02", L"P1_Attack02", TIMELINE_EVENT(47.0, [this]() {
		CGameInstance* pGameInstance = CGameInstance::GetInstance();
		Safe_AddRef(pGameInstance);

		_vector vPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
		_vector vLook = m_pTransformCom->Get_State(CTransform::STATE_LOOK);

		P1Attack02* pBullet;
		P1Attack02::tagP1Attack02Desc tResetDesc;
		vPosition += XMVector3Normalize(vLook);
		tResetDesc.fDamage = 10.f;
		tResetDesc.pOwner = this;
		DirectX::XMStoreFloat4(&tResetDesc.vPosition, vPosition);

		pBullet = ObjectPool<P1Attack02>::GetInstance()->PopPool(P1Attack02::ProtoTag(), &tResetDesc);
		pGameInstance->AddToLayer(pGameInstance->Get_CurLevelIndex(), L"Layer_Bullet", pBullet);

		Safe_Release(pGameInstance);

		}));


	m_pModelCom->Add_TimeLineEvent("Queen_Moggoth_P1_Attack03", L"P1_Attack03", TIMELINE_EVENT(80.0, [this]() {
		CGameInstance* pGameInstance = CGameInstance::GetInstance();
		Safe_AddRef(pGameInstance);

		P1Attack03* pBullet;
		P1Attack03::tagP1Attack03Desc tResetDesc;
		tResetDesc.fDamage = 10.f;
		tResetDesc.pOwner = this;
		_vector vPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

		for (_uint i = 0; i < 10; ++i)
		{
			DirectX::XMStoreFloat4(&tResetDesc.vPosition, vPosition);
			_int iRadiusX = RandomIntFrom_A_To_B(-10, 10);
			_int iRadiusZ = RandomIntFrom_A_To_B(-10, 10);
			tResetDesc.vPosition.x = tResetDesc.vPosition.x + iRadiusX;
			tResetDesc.vPosition.z = tResetDesc.vPosition.z + iRadiusZ;
			pBullet = ObjectPool<P1Attack03>::GetInstance()->PopPool(P1Attack03::ProtoTag(), &tResetDesc);
			pGameInstance->AddToLayer(pGameInstance->Get_CurLevelIndex(), L"Layer_Bullet", pBullet);
		}

		Safe_Release(pGameInstance);

		}));

	const _tchar* pTag[31] = { L"P1_Attack04_0", L"P1_Attack04_1", L"P1_Attack04_2", L"P1_Attack04_3", L"P1_Attack04_4", L"P1_Attack04_5",
	L"P1_Attack04_6", L"P1_Attack04_7", L"P1_Attack04_8", L"P1_Attack04_9",
	L"P1_Attack04_10", L"P1_Attack04_11", L"P1_Attack04_12", L"P1_Attack04_13", L"P1_Attack04_14", L"P1_Attack04_15",
	L"P1_Attack04_16", L"P1_Attack04_17", L"P1_Attack04_18", L"P1_Attack04_19",
	L"P1_Attack04_20", L"P1_Attack04_21", L"P1_Attack04_22", L"P1_Attack04_23", L"P1_Attack04_24", L"P1_Attack04_25",
	L"P1_Attack04_26", L"P1_Attack04_27", L"P1_Attack04_28", L"P1_Attack04_29", L"P1_Attack04_30" };
	for (_uint i = 1; i <= 30; ++i)
	{
		m_pModelCom->Add_TimeLineEvent("Queen_Moggoth_P1_Attack04_loop", pTag[i], TIMELINE_EVENT(1.5 * i, [this]() {
			CGameInstance* pGameInstance = CGameInstance::GetInstance();
			Safe_AddRef(pGameInstance);

			P1Attack04* pBullet;
			P1Attack04::tagP1Attack04Desc tResetDesc;
			tResetDesc.fDamage = 10.f;
			tResetDesc.pOwner = this;
			_vector vPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

			
			DirectX::XMStoreFloat4(&tResetDesc.vPosition, vPosition);
			_int iRadiusX = RandomIntFrom_A_To_B(-10, 10);
			_int iRadiusZ = RandomIntFrom_A_To_B(-10, 10);
			tResetDesc.vPosition.x = tResetDesc.vPosition.x + iRadiusX;
			tResetDesc.vPosition.z = tResetDesc.vPosition.z + iRadiusZ;
			pBullet = ObjectPool<P1Attack04>::GetInstance()->PopPool(P1Attack04::ProtoTag(), &tResetDesc);
			pGameInstance->AddToLayer(pGameInstance->Get_CurLevelIndex(), L"Layer_Bullet", pBullet);
			

			Safe_Release(pGameInstance);

			}));
	}

	// 꼬리푹찍
	m_pModelCom->Add_TimeLineEvent("Queen_Moggoth_P2_Attack01", L"P2_Attack01", TIMELINE_EVENT(34.0, [this]() {
		CGameInstance* pGameInstance = CGameInstance::GetInstance();
		Safe_AddRef(pGameInstance);

		_vector vPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
		_vector vLook = m_pTransformCom->Get_State(CTransform::STATE_LOOK);

		P2Attack01* pBullet;
		P2Attack01::tagP2Attack01Desc tResetDesc;
		tResetDesc.fDamage = 10.f;
		tResetDesc.pOwner = this;
		vPosition += XMVector3Normalize(vLook) * 4.f;

		DirectX::XMStoreFloat4(&tResetDesc.vPosition, vPosition);
		
		pBullet = ObjectPool<P2Attack01>::GetInstance()->PopPool(P2Attack01::ProtoTag(), &tResetDesc);
		pGameInstance->AddToLayer(pGameInstance->Get_CurLevelIndex(), L"Layer_Bullet", pBullet);

		Safe_Release(pGameInstance);

		}));

	// 덜덜거리면서 땅에 뭐올라옴
	const _tchar* pTag2[31] = { L"P2_Attack02_0", L"P2_Attack02_1", L"P2_Attack02_2", L"P2_Attack02_3", L"P2_Attack02_4", L"P2_Attack02_5",
	L"P2_Attack02_6", L"P2_Attack02_7", L"P2_Attack02_8", L"P2_Attack02_9",
	L"P2_Attack02_10", L"P2_Attack02_11", L"P2_Attack02_12", L"P2_Attack02_13", L"P2_Attack02_14", L"P2_Attack02_15",
	L"P2_Attack02_16", L"P2_Attack02_17", L"P2_Attack02_18", L"P2_Attack02_19",
	L"P2_Attack02_20", L"P2_Attack02_21", L"P2_Attack02_22", L"P2_Attack02_23", L"P2_Attack02_24", L"P2_Attack02_25",
	L"P2_Attack02_26", L"P2_Attack02_27", L"P2_Attack02_28", L"P2_Attack02_29", L"P2_Attack02_30" };
	for (_uint i = 1; i <= 30; ++i)
	{
		m_pModelCom->Add_TimeLineEvent("Queen_Moggoth_P2_Attack02_loop", pTag2[i], TIMELINE_EVENT(1.5 * i, [this]() {
			CGameInstance* pGameInstance = CGameInstance::GetInstance();
			Safe_AddRef(pGameInstance);

			P2Attack02* pBullet;
			P2Attack02::tagP2Attack02Desc tResetDesc;
			tResetDesc.fDamage = 10.f;
			tResetDesc.pOwner = this;
			_vector vPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

			DirectX::XMStoreFloat4(&tResetDesc.vPosition, vPosition);
			_int iRadiusX = RandomIntFrom_A_To_B(-10, 10);
			_int iRadiusZ = RandomIntFrom_A_To_B(-10, 10);
			tResetDesc.vPosition.x = tResetDesc.vPosition.x + iRadiusX;
			tResetDesc.vPosition.z = tResetDesc.vPosition.z + iRadiusZ;
			pBullet = ObjectPool<P2Attack02>::GetInstance()->PopPool(P2Attack02::ProtoTag(), &tResetDesc);
			pGameInstance->AddToLayer(pGameInstance->Get_CurLevelIndex(), L"Layer_Bullet", pBullet);

			Safe_Release(pGameInstance);

			}));
	}

	// 꼬리 휘두르기
	m_pModelCom->Add_TimeLineEvent("Queen_Moggoth_P2_Attack03", L"P2_Attack03", TIMELINE_EVENT(30.0, [this]() {
		CGameInstance* pGameInstance = CGameInstance::GetInstance();
		Safe_AddRef(pGameInstance);

		_vector vPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
		_vector vLook = m_pTransformCom->Get_State(CTransform::STATE_LOOK);

		P2Attack03* pBullet;
		P2Attack03::tagP2Attack03Desc tResetDesc;
		tResetDesc.fDamage = 10.f;
		tResetDesc.pOwner = this;
		vPosition -= XMVector3Normalize(vLook) * 4.f;

		DirectX::XMStoreFloat4(&tResetDesc.vPosition, vPosition);

		pBullet = ObjectPool<P2Attack03>::GetInstance()->PopPool(P2Attack03::ProtoTag(), &tResetDesc);
		pGameInstance->AddToLayer(pGameInstance->Get_CurLevelIndex(), L"Layer_Bullet", pBullet);
		

		Safe_Release(pGameInstance);

		}));

	//m_pModelCom->Add_TimeLineEvent("Queen_Moggoth_P2_Attack04_shot", L"P2_Attack04", TIMELINE_EVENT(5.0, [this]() {
	//	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	//	Safe_AddRef(pGameInstance);

	//	_vector vPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	//	P2Attack04* pBullet;
	//	P2Attack04::tagP2Attack04Desc tResetDesc;

	//	vPosition.m128_f32[1] += 4.8f;
	//	cout << "발사" << endl;
	//	tResetDesc.fDamage = 10.f;
	//	tResetDesc.pOwner = this;
	//	DirectX::XMStoreFloat4(&tResetDesc.vPosition, vPosition);
	//	tResetDesc.vTargetPos = Single->GetClintPosition();

	//	pBullet = ObjectPool<P2Attack04>::GetInstance()->PopPool(P2Attack04::ProtoTag(), &tResetDesc);
	//	pGameInstance->AddToLayer(pGameInstance->Get_CurLevelIndex(), L"Layer_Bullet", pBullet);

	//	Safe_Release(pGameInstance);

	//	}));

	

	return S_OK;
}

HRESULT Queen_Moggoth::SetUp_ShaderResources()
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

Queen_Moggoth* Queen_Moggoth::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	Queen_Moggoth* pInstance = new Queen_Moggoth(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created Queen_Moggoth");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* Queen_Moggoth::Clone(void* pArg)
{
	Queen_Moggoth* pInstance = new Queen_Moggoth(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned Queen_Moggoth");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void Queen_Moggoth::Free(void)
{
	__super::Free();

	--Queen_Moggoth_Id;

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pColliderCom);
	Safe_Release(m_pAttack04CollCom);
	Safe_Release(m_pStateContextCom);
	Safe_Release(m_pNavigationCom);
	Safe_Release(m_pRaycastCom);
	Safe_Release(m_pHealthCom);
	/* Don't Forget Release for the VIBuffer or Model Component*/
}
