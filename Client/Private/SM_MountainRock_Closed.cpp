#include "SM_MountainRock_Closed.h"
#include "GameInstance.h"

_uint SM_MountainRock_Closed::SM_MountainRock_Closed_Id = 0;

SM_MountainRock_Closed::SM_MountainRock_Closed(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject(pDevice, pContext)
{
}

SM_MountainRock_Closed::SM_MountainRock_Closed(const SM_MountainRock_Closed& rhs)
	: CGameObject(rhs)
{
}

HRESULT SM_MountainRock_Closed::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}


HRESULT SM_MountainRock_Closed::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Add_Components()))
		return E_FAIL;

	++SM_MountainRock_Closed_Id;
	m_tInfo.wstrName = TO_WSTR("SM_MountainRock_Closed" + to_string(SM_MountainRock_Closed_Id));
	m_tInfo.wstrKey = ProtoTag();
	m_tInfo.ID = SM_MountainRock_Closed_Id;

	tagSM_MountainRock_ClosedDesc tCloneDesc;
	if (nullptr != pArg)
		tCloneDesc = *(tagSM_MountainRock_ClosedDesc*)pArg;
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMLoadFloat4(&tCloneDesc.vPosition));

	//m_pModelCom->Set_RootNode(3);

	return S_OK;
}

void SM_MountainRock_Closed::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);

	// 	m_pModelCom->Play_Animation(TimeDelta);
	// 	if(nullptr != m_pColliderCom)
	//		m_pColliderCom->Tick(m_pTransformCom->Get_WorldMatrix());
}

void SM_MountainRock_Closed::Late_Tick(_double TimeDelta)
{
	__super::Late_Tick(TimeDelta);

	if (Facade->isRender(m_pRendererCom, m_pTransformCom))
	{
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this);
	}
	// m_pColliderCom->Add_ColliderGroup(COLL_GROUP::);
#ifdef _DEBUG
	// m_pRendererCom->Add_DebugGroup(m_pNavigationCom);
	//if (nullptr != m_pColliderCom)
	//	m_pRendererCom->Add_DebugGroup(m_pColliderCom);
	// m_pRendererCom->Add_DebugGroup(m_pRaycastCom);
#endif
}

HRESULT SM_MountainRock_Closed::Render()
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
		// m_pModelCom->Bind_Material(m_pShaderCom, "g_NormalTexture", i, TextureType_NORMALS);

		m_pShaderCom->Begin(0);

		m_pModelCom->Render(i);
	}

	// 만약에 모델 컴포넌트 안쓰면 이걸로 쓰면된다.
	// m_pShaderCom->Begin(0);
}

void SM_MountainRock_Closed::Save(HANDLE hFile, DWORD& dwByte)
{
	m_tInfo.Save(hFile, dwByte);
	m_pTransformCom->Save(hFile, dwByte);
}

void SM_MountainRock_Closed::Load(HANDLE hFile, DWORD& dwByte, _uint iLevelIndex)
{
	m_pTransformCom->Load(hFile, dwByte, iLevelIndex);
}

HRESULT SM_MountainRock_Closed::Add_Components()
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
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_STATIC, L"Prototype_Component_Shader_VtxMesh", L"Com_Shader", (CComponent**)&m_pShaderCom, &tShaderDesc), E_FAIL);

	CModel::CMODEL_DESC tModelDesc; tModelDesc.pOwner = this;
	FAILED_CHECK_RETURN(__super::Add_Component(eLevelID, L"Prototype_Component_Model_SM_MountainRock_Closed", L"Com_Model", (CComponent**)&m_pModelCom, &tModelDesc), E_FAIL);

	CColliderAABB::CCOLLIDER_AABB_DESC tColliderAABBDesc;
	tColliderAABBDesc.pOwner = this;
	tColliderAABBDesc.Extents = _float3(0.3f, 1.f, 0.3f);
	tColliderAABBDesc.vCenter = _float3(-0.15f, tColliderAABBDesc.Extents.y, 0.f);
	//FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_STATIC, CColliderAABB::ProtoTag(), L"Com_BodyColl", (CComponent**)&m_pColliderCom, &tColliderAABBDesc), E_FAIL);

	CNavigation::CNAVIGATION_DESC tNavigationdesc; tNavigationdesc.pOwner = this;
	tNavigationdesc.iCurrentIndex = { 0 };
	//FAILED_CHECK_RETURN(__super::Add_Component(eLevelID, CNavigation::ProtoTag(), L"Com_Navigation", (CComponent**)&m_pNavigationCom, &tNavigationdesc), E_FAIL);

	Safe_Release(pGameInstance);
	return S_OK;
}

HRESULT SM_MountainRock_Closed::SetUp_ShaderResources()
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

SM_MountainRock_Closed* SM_MountainRock_Closed::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	SM_MountainRock_Closed* pInstance = new SM_MountainRock_Closed(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created SM_MountainRock_Closed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* SM_MountainRock_Closed::Clone(void* pArg)
{
	SM_MountainRock_Closed* pInstance = new SM_MountainRock_Closed(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned SM_MountainRock_Closed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void SM_MountainRock_Closed::Free(void)
{
	__super::Free();

	--SM_MountainRock_Closed_Id;
	
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pRendererCom);
	/* Don't Forget Release for the VIBuffer or Model Component*/
}

