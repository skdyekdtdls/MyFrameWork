#include "Alien_prawn.h"
#include "GameInstance.h"
#include "Alien_prawnIdle.h"
#include "StateContext.h"
_uint Alien_prawn::Alien_prawn_Id = 0;

/* Don't Forget Release for the VIBuffer or Model Component*/

Alien_prawn::Alien_prawn(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject(pDevice, pContext)
{
}

Alien_prawn::Alien_prawn(const Alien_prawn& rhs)
	: CGameObject(rhs)
{
}

HRESULT Alien_prawn::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT Alien_prawn::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Add_Components()))
		return E_FAIL;

	++Alien_prawn_Id;
	m_tInfo.wstrName = TO_WSTR("Alien_prawn" + to_string(Alien_prawn_Id));
	m_tInfo.wstrKey = ProtoTag();
	m_tInfo.ID = Alien_prawn_Id;

	ALIEN_PRAWN_DESC tCloneDesc;
	tCloneDesc.vPosition = _float4(0.f, 0.f, 0.f, 1.f);
	if (nullptr != pArg)
		tCloneDesc = *(ALIEN_PRAWN_DESC*)pArg;
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMLoadFloat4(&tCloneDesc.vPosition));
	
	// ���� �ʱ�ȭ
	m_pStateContextCom->TransitionTo(L"Alien_prawnIdle");
	return S_OK;
}

void Alien_prawn::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);

	m_pModelCom->Play_Animation(TimeDelta);

	if (nullptr != m_pColliderCom)
		m_pColliderCom->Tick(m_pTransformCom->Get_WorldMatrix());

	if (nullptr != m_pRaycastCom)
	{
		m_pRaycastCom->Tick(m_pTransformCom->Get_State(CTransform::STATE_POSITION),
			m_pTransformCom->Get_State(CTransform::STATE_LOOK));
	}

	if (nullptr != m_pStateContextCom)
		m_pStateContextCom->Tick(TimeDelta);
}

void Alien_prawn::Late_Tick(_double TimeDelta)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	__super::Late_Tick(TimeDelta);

	if(pGameInstance->isIn_WorldSpace(m_pTransformCom->Get_State(CTransform::STATE_POSITION), 1.f))
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this);

	CColliderAABB* pClintAABB = static_cast<CColliderAABB*>(
		pGameInstance->Get_ComponentOfClone(pGameInstance->Get_CurLevelIndex(), L"Layer_Player",
		"Clint1", L"Com_BodyColl"));

	pClintAABB->Intersect(m_pRaycastCom);
	m_pRaycastCom->Intersect(pClintAABB);
	Safe_Release(pGameInstance);
}

HRESULT Alien_prawn::Render()
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
		// m_pModelCom->Bind_Material(m_pShaderCom, "g_NormalTexture", i, aiTextureType_NORMALS);

		m_pShaderCom->Begin(0);

		m_pModelCom->Render(i);
	}

#ifdef _DEBUG
	 if (nullptr != m_pColliderCom)
		m_pColliderCom->Render();
	 if (nullptr != m_pRaycastCom)
		m_pRaycastCom->Render();
#endif
}

void Alien_prawn::OnCollision(CCollider::COLLISION_INFO* pCollisionInfo)
{
	if (L"Prototype_GameObject_Pistola" == pCollisionInfo->tInfo.wstrKey)
	{
		m_pStateContextCom->TransitionTo(TEXT("Alien_prawnDead"));
	}
}

void Alien_prawn::Save(HANDLE hFile, DWORD& dwByte)
{
	m_tInfo.Save(hFile, dwByte);
	m_pTransformCom->Save(hFile, dwByte);
}

void Alien_prawn::Load(HANDLE hFile, DWORD& dwByte, _uint iLevelIndex)
{
	m_pTransformCom->Load(hFile, dwByte, iLevelIndex);
}

HRESULT Alien_prawn::Add_Components()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);
	LEVELID eLevelID = static_cast<LEVELID>(pGameInstance->Get_NextLevelIndex());

	/* For.Com_Renderer */
	CRenderer::CRENDERER_DESC tRendererDesc; tRendererDesc.pOwner = this;
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_STATIC, CRenderer::ProtoTag(), L"Com_Renderer", (CComponent**)&m_pRendererCom, &tRendererDesc), E_FAIL);
	// no texture now, you have to add texture later

	CTransform::CTRANSFORM_DESC TransformDesc{ 7.0, XMConvertToRadians(720.f) }; TransformDesc.pOwner = this;
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_STATIC, CTransform::ProtoTag(), L"Com_Transform", (CComponent**)&m_pTransformCom
		, &TransformDesc), E_FAIL);

	CShader::CSHADER_DESC tShaderDesc; tShaderDesc.pOwner = this;
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_STATIC, L"Prototype_Component_Shader_VtxAnimMesh", L"Com_Shader", (CComponent**)&m_pShaderCom, &tShaderDesc), E_FAIL);

	CModel::CMODEL_DESC tModelDesc; tModelDesc.pOwner = this;
	FAILED_CHECK_RETURN(__super::Add_Component(eLevelID, L"Prototype_Component_Model_Alien_prawn", L"Com_Model", (CComponent**)&m_pModelCom, &tModelDesc), E_FAIL);

	CColliderAABB::CCOLLIDER_AABB_DESC tColliderAABBDesc;
	tColliderAABBDesc.pOwner = this;
	tColliderAABBDesc.Extents = _float3(0.3f, 1.f, 0.3f);
	tColliderAABBDesc.vCenter = _float3(0.f, tColliderAABBDesc.Extents.y, 0.f);
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_STATIC, CColliderAABB::ProtoTag(), L"Com_BodyColl", (CComponent**)&m_pColliderCom, &tColliderAABBDesc), E_FAIL);

	CNavigation::CNAVIGATION_DESC tNavigationdesc; tNavigationdesc.pOwner = this;
	tNavigationdesc.iCurrentIndex = { 0 };
	FAILED_CHECK_RETURN(__super::Add_Component(eLevelID, CNavigation::ProtoTag(), L"Com_Navigation", (CComponent**)&m_pNavigationCom, &tNavigationdesc), E_FAIL);

	Raycast::RAYCAST_DESC tRaycastDesc;
	tRaycastDesc.pOwner = this;
	tRaycastDesc.vCenter = _float3(0.15f, 1.f, 0.f);
	tRaycastDesc.fLength = { 1.f };
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_STATIC, Raycast::ProtoTag(), L"Com_RayDetect", (CComponent**)&m_pRaycastCom, &tRaycastDesc), E_FAIL);

	AlienPrawnState::STATE_CONTEXT_DESC tStateContextDesc;
	tStateContextDesc.pOwner = this;
	FAILED_CHECK_RETURN(__super::Add_Component(eLevelID, TEXT("Prototype_Component_AlienPrawnState"), L"Com_AlienPrawnState", (CComponent**)&m_pStateContextCom, &tStateContextDesc), E_FAIL);

	Safe_Release(pGameInstance);
	return S_OK;
}

HRESULT Alien_prawn::SetUp_ShaderResources()
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

Alien_prawn* Alien_prawn::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	Alien_prawn* pInstance = new Alien_prawn(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created Alien_prawn");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* Alien_prawn::Clone(void* pArg)
{
	Alien_prawn* pInstance = new Alien_prawn(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned Alien_prawn");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void Alien_prawn::Free(void)
{
	__super::Free();

	--Alien_prawn_Id;
	Safe_Release(m_pNavigationCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pColliderCom);
	Safe_Release(m_pRaycastCom);
	Safe_Release(m_pStateContextCom);

	/* Don't Forget Release for the VIBuffer or Model Component*/
}
