#include "ForkLift.h"
#include "GameInstance.h"

/* Don't Forget Release for the VIBuffer or Model Component*/
_uint ForkLift::CPlayer_Id = 0;

ForkLift::ForkLift(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject(pDevice, pContext)
{
}

ForkLift::ForkLift(const ForkLift& rhs)
	: CGameObject(rhs)
{
}

void ForkLift::Save(HANDLE hFile, DWORD& dwByte)
{
	m_tInfo.Save(hFile, dwByte);
	m_pTransformCom->Save(hFile, dwByte);
}

void ForkLift::Load(HANDLE hFile, DWORD& dwByte, _uint iLevelIndex)
{
	m_pTransformCom->Load(hFile, dwByte, iLevelIndex);
}

HRESULT ForkLift::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}


HRESULT ForkLift::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Add_Components()))
		return E_FAIL;

	++CPlayer_Id;
	m_tInfo.wstrName = TO_WSTR("ForkLift" + to_string(CPlayer_Id));
	m_tInfo.wstrKey = ProtoTag();
	m_tInfo.ID = CPlayer_Id;

	return S_OK;
}

void ForkLift::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);
	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this);
}

void ForkLift::Late_Tick(_double TimeDelta)
{
	__super::Late_Tick(TimeDelta);
	
}

HRESULT ForkLift::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	if (FAILED(SetUp_ShaderResources()))
		return E_FAIL;

	_uint		iNumMeshes = m_pModelCom->Get_NumMeshes();
	for (size_t i = 0; i < iNumMeshes; i++)
	{
		m_pModelCom->Bind_Material(m_pShaderCom, "g_DiffuseTexture", i, TextureType_DIFFUSE);
		// m_pModelCom->Bind_Material(m_pShaderCom, "g_NormalTexture", i, aiTextureType_NORMALS);

		m_pShaderCom->Begin(0);

		m_pModelCom->Render(i);
	}
}

HRESULT ForkLift::Add_Components()
{
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_STATIC, CRenderer::ProtoTag(), L"Com_Renderer", (CComponent**)&m_pRendererCom), E_FAIL);

	// no texture now, you have to add texture later

	CTransform::TRANSFORMDESC TransformDesc{ 7.0, XMConvertToRadians(90.f) };
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_STATIC, CTransform::ProtoTag(), L"Com_Transform", (CComponent**)&m_pTransformCom
		, &TransformDesc), E_FAIL);
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_STATIC, L"Prototype_Component_Shader_VtxMesh", L"Com_Shader", (CComponent**)&m_pShaderCom), E_FAIL);
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_IMGUI , L"Prototype_Component_Model_Fiona", L"Com_Model", (CComponent**)&m_pModelCom), E_FAIL);

	// You can Add VIBuffer or Model Component
	return S_OK;
}

HRESULT ForkLift::SetUp_ShaderResources()
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

ForkLift* ForkLift::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	ForkLift* pInstance = new ForkLift(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created ForkLift");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* ForkLift::Clone(void* pArg)
{
	ForkLift* pInstance = new ForkLift(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned ForkLift");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void ForkLift::Free(void)
{
	__super::Free();

	Safe_Release(m_pTextureCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModelCom);
}

