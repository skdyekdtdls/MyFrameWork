
#include "MineCrystalsD.h"
#include "GameInstance.h"

_uint MineCrystalsD::MineCrystalsD_Id = 0;

MineCrystalsD::MineCrystalsD(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject(pDevice, pContext)
{
}

MineCrystalsD::MineCrystalsD(const MineCrystalsD& rhs)
	: CGameObject(rhs)
{
}

HRESULT MineCrystalsD::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}


HRESULT MineCrystalsD::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Add_Components()))
		return E_FAIL;

	++MineCrystalsD_Id;
	m_tInfo.wstrName = TO_WSTR("MineCrystalsD" + to_string(MineCrystalsD_Id));
	m_tInfo.wstrKey = ProtoTag();
	m_tInfo.ID = MineCrystalsD_Id;

	tagMineCrystalsDDesc tCloneDesc;
	if (nullptr != pArg)
		tCloneDesc = *(tagMineCrystalsDDesc*)pArg;
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMLoadFloat4(&tCloneDesc.vPosition));

	//m_pModelCom->Set_RootNode(3);

	return S_OK;
}

void MineCrystalsD::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);

	// 	m_pModelCom->Play_Animation(TimeDelta);
	// 	if(nullptr != m_pColliderCom)
	//		m_pColliderCom->Tick(m_pTransformCom->Get_WorldMatrix());
}

void MineCrystalsD::Late_Tick(_double TimeDelta)
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

HRESULT MineCrystalsD::Render()
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

	// ���࿡ �� ������Ʈ �Ⱦ��� �̰ɷ� ����ȴ�.
	// m_pShaderCom->Begin(0);
}

void MineCrystalsD::Save(HANDLE hFile, DWORD& dwByte)
{
	m_tInfo.Save(hFile, dwByte);
	m_pTransformCom->Save(hFile, dwByte);
}

void MineCrystalsD::Load(HANDLE hFile, DWORD& dwByte, _uint iLevelIndex)
{
	m_pTransformCom->Load(hFile, dwByte, iLevelIndex);
}

HRESULT MineCrystalsD::Add_Components()
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
	FAILED_CHECK_RETURN(__super::Add_Component(eLevelID, L"Prototype_Component_Model_MineCrystalsD", L"Com_Model", (CComponent**)&m_pModelCom, &tModelDesc), E_FAIL);

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

HRESULT MineCrystalsD::SetUp_ShaderResources()
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

MineCrystalsD* MineCrystalsD::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	MineCrystalsD* pInstance = new MineCrystalsD(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created MineCrystalsD");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* MineCrystalsD::Clone(void* pArg)
{
	MineCrystalsD* pInstance = new MineCrystalsD(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned MineCrystalsD");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void MineCrystalsD::Free(void)
{
	__super::Free();

	--MineCrystalsD_Id;
	
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pRendererCom);
	/* Don't Forget Release for the VIBuffer or Model Component*/
}
