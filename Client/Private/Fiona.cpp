#include "..\Public\Fiona.h"
#include "GameInstance.h"

_uint Fiona::Fiona_Id = 0;

Fiona::Fiona(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject(pDevice, pContext)
{
}

Fiona::Fiona(const Fiona& rhs)
	: CGameObject(rhs)
{
}

HRESULT Fiona::Initialize_Prototype()
{
	/* 서버로 부터 받아와야할 데이터를 셋팅하낟. */
	/* 파일로부터  받아와야할 데이터를 셋팅하낟. */
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT Fiona::Initialize(void* pArg)
{
	/* 원형생성할 때 받아왔던 데이터 외의 추가적인 값들을 더 저장해주낟. */
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Add_Components()))
		return E_FAIL;

	++Fiona_Id;
	m_tInfo.wstrName = TO_WSTR("Fiona" + to_string(Fiona_Id));
	m_tInfo.wstrKey = ProtoTag();
	m_tInfo.ID = Fiona_Id;

	CLONE_DESC tCloneDesc;
	ZeroStruct(tCloneDesc);
	tCloneDesc.vPosition = _float4(0.f, 0.f, 0.f, 1.f);
	if (nullptr != pArg)
		tCloneDesc = *(CLONE_DESC*)pArg;
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMLoadFloat4(&tCloneDesc.vPosition));
	
	m_pModelCom->Set_AnimByIndex(rand() % 10);

	return S_OK;
}

void Fiona::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);

	KeyInput(TimeDelta);

	m_pModelCom->Play_Animation(TimeDelta);

	//m_pTransformCom->Rotation(XMVectorSet(1.f, 0.f, 0.f, 1.f), XMConvertToRadians(77.f));

	NULL_CHECK(m_pRendererCom);
	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this);

	if(nullptr != m_pColliderCom)
		m_pColliderCom->Tick(m_pTransformCom->Get_WorldMatrix());
}

void Fiona::Late_Tick(_double TimeDelta)
{
	__super::Late_Tick(TimeDelta);
}

HRESULT Fiona::Render()
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
	if(nullptr != m_pColliderCom)
		m_pColliderCom->Render();
#endif

	return S_OK;
}

void Fiona::Save(HANDLE hFile, DWORD& dwByte)
{
	m_tInfo.Save(hFile, dwByte);
	m_pTransformCom->Save(hFile, dwByte);
}

void Fiona::Load(HANDLE hFile, DWORD& dwByte, _uint iLevelIndex)
{
	m_pTransformCom->Load(hFile, dwByte, iLevelIndex);
}

HRESULT Fiona::Add_Components()
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
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_STATIC, L"Prototype_Component_Shader_VtxAnimMesh", L"Com_Shader", (CComponent**)&m_pShaderCom), E_FAIL);
	FAILED_CHECK_RETURN(__super::Add_Component(eLevelID, L"Prototype_Component_Model_Fiona", L"Com_Model", (CComponent**)&m_pModelCom), E_FAIL);
	CNavigation::NAVIGATIONDESC tNavigationdesc;
	tNavigationdesc.iCurrentIndex = { 0 };
	FAILED_CHECK_RETURN(__super::Add_Component(eLevelID, CNavigation::ProtoTag(), L"Com_Navigation", (CComponent**)&m_pNavigationCom, &tNavigationdesc), E_FAIL);

	Safe_Release(pGameInstance);
	return S_OK;
}

HRESULT Fiona::SetUp_ShaderResources()
{
	_float4x4 MyMatrix = m_pTransformCom->Get_WorldFloat4x4();
	FAILED_CHECK_RETURN(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &MyMatrix), E_FAIL);

	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	MyMatrix = pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_VIEW);
	FAILED_CHECK_RETURN(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &MyMatrix), E_FAIL);

	MyMatrix = pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_PROJ);
	FAILED_CHECK_RETURN(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &MyMatrix), E_FAIL);

	_float4 MyFloat4 = pGameInstance->Get_CamPosition();
	FAILED_CHECK_RETURN(m_pShaderCom->Bind_RawValue("g_vCamPosition", &MyFloat4, sizeof(_float4)), E_FAIL);
	
	Safe_Release(pGameInstance);

	return S_OK;
}

void Fiona::KeyInput(_double& TimeDelta)
{
	if (g_hWnd != ::GetFocus())
		return;

	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	if (pGameInstance->Get_DIKeyState(DIK_UPARROW))
		m_pTransformCom->Go_Straight(TimeDelta, m_pNavigationCom);

	if (pGameInstance->Get_DIKeyState(DIK_DOWNARROW))
		m_pTransformCom->Go_Backward(TimeDelta, m_pNavigationCom);

	if (pGameInstance->Get_DIKeyState(DIK_RIGHTARROW))
		m_pTransformCom->Go_Right(TimeDelta, m_pNavigationCom);

	if (pGameInstance->Get_DIKeyState(DIK_LEFTARROW))
		m_pTransformCom->Go_Left(TimeDelta, m_pNavigationCom);

	if (pGameInstance->Get_DIKeyState(DIK_E))
		m_pTransformCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), TimeDelta);

	if (pGameInstance->Get_DIKeyState(DIK_Q))
		m_pTransformCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), -TimeDelta);

	Safe_Release(pGameInstance);
}

Fiona* Fiona::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	Fiona* pInstance = new Fiona(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created Fiona");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* Fiona::Clone(void* pArg)
{
	Fiona* pInstance = new Fiona(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned Fiona");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void Fiona::Free()
{
	__super::Free();
	--Fiona_Id;
	Safe_Release(m_pNavigationCom);
	Safe_Release(m_pColliderCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pRendererCom);

}