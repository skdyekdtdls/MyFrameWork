
#include "RailSupportB.h"
#include "GameInstance.h"

_uint RailSupportB::RailSupportB_Id = 0;

/* Don't Forget Release for the VIBuffer or Model Component*/

RailSupportB::RailSupportB(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject(pDevice, pContext)
{
}

RailSupportB::RailSupportB(const RailSupportB& rhs)
	: CGameObject(rhs)
{
}

HRESULT RailSupportB::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}


HRESULT RailSupportB::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Add_Components()))
		return E_FAIL;

	++RailSupportB_Id;
	m_tInfo.wstrName = TO_WSTR("RailSupportB" + to_string(RailSupportB_Id));
	m_tInfo.wstrKey = ProtoTag();
	m_tInfo.ID = RailSupportB_Id;

	CLONE_DESC tCloneDesc;
	if (nullptr != pArg)
		tCloneDesc = *(CLONE_DESC*)pArg;
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMLoadFloat4(&tCloneDesc.vPosition));

	return S_OK;
}

void RailSupportB::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);

	// 	m_pModelCom->Play_Animation(TimeDelta);
	 	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this);
	// 	if(nullptr != m_pColliderCom)
	//		m_pColliderCom->Tick(m_pTransformCom->Get_WorldMatrix());
}

void RailSupportB::Late_Tick(_double TimeDelta)
{
	__super::Late_Tick(TimeDelta);
}

HRESULT RailSupportB::Render()
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

	// 만약에 모델 컴포넌트 안쓰면 이걸로 쓰면된다.
	// m_pShaderCom->Begin(0);

#ifdef _DEBUG
	// if(nullptr != m_pColliderCom)
	//	m_pColliderCom->Render();
#endif
}

void RailSupportB::Save(HANDLE hFile, DWORD& dwByte)
{
	m_tInfo.Save(hFile, dwByte);
	m_pTransformCom->Save(hFile, dwByte);
}

void RailSupportB::Load(HANDLE hFile, DWORD& dwByte, _uint iLevelIndex)
{
	m_pTransformCom->Load(hFile, dwByte, iLevelIndex);
}

HRESULT RailSupportB::Add_Components()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);
	LEVELID eLevelID = static_cast<LEVELID>(pGameInstance->Get_NextLevelIndex());

	/* For.Com_Renderer */
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_STATIC, CRenderer::ProtoTag(), L"Com_Renderer", (CComponent**)&m_pRendererCom), E_FAIL);

	// no texture now, you have to add texture later

	CTransform::TRANSFORMDESC TransformDesc{ 7.0, XMConvertToRadians(90.f) };
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_STATIC, CTransform::ProtoTag(), L"Com_Transform", (CComponent**)&m_pTransformCom
		, &TransformDesc), E_FAIL);
	 FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_STATIC, L"Prototype_Component_Shader_VtxMesh", L"Com_Shader", (CComponent**)&m_pShaderCom), E_FAIL);
	 FAILED_CHECK_RETURN(__super::Add_Component(eLevelID, L"Prototype_Component_Model_RailSupportB", L"Com_Model", (CComponent**)&m_pModelCom), E_FAIL);
	// CNavigation::NAVIGATIONDESC tNavigationdesc;
	// tNavigationdesc.iCurrentIndex = { 0 };
	// FAILED_CHECK_RETURN(__super::Add_Component(eLevelID, CNavigation::ProtoTag(), L"Com_Navigation", (CComponent**)&m_pNavigationCom, &tNavigationdesc), E_FAIL);

	Safe_Release(pGameInstance);
	return S_OK;
}

HRESULT RailSupportB::SetUp_ShaderResources()
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

RailSupportB* RailSupportB::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	RailSupportB* pInstance = new RailSupportB(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created RailSupportB");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* RailSupportB::Clone(void* pArg)
{
	RailSupportB* pInstance = new RailSupportB(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned RailSupportB");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void RailSupportB::Free(void)
{
	__super::Free();

	--RailSupportB_Id;
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pRendererCom);
	/* Don't Forget Release for the VIBuffer or Model Component*/
}

