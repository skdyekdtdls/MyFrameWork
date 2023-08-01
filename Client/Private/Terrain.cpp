#include "Terrain.h"
#include "GameInstance.h"
#ifdef _DEBUG
#include "ImWindow_Manager.h"
#include "ImWindow_MapTool.h"
#endif

_uint CTerrain::CTerrain_Id = { 0 };

CTerrain::CTerrain(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject(pDevice, pContext)
{

}

CTerrain::CTerrain(const CTerrain& rhs)
	: CGameObject(rhs)
{
}

HRESULT CTerrain::Initialize_Prototype()
{
	FAILED_CHECK_RETURN(__super::Initialize_Prototype(), E_FAIL);

	return S_OK;
}

HRESULT CTerrain::Initialize(void* pArg)
{
	FAILED_CHECK_RETURN(__super::Initialize(pArg), E_FAIL);

	if (FAILED(Add_Components()))
		return E_FAIL;

	++CTerrain_Id;
	m_tInfo.wstrName = TO_WSTR("CTerrain" + to_string(CTerrain_Id));
	m_tInfo.wstrKey = ProtoTag();
	m_tInfo.ID = CTerrain_Id;

	return S_OK;
}

void CTerrain::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);

	RAY tRay = Single->GetMouseRay(m_pContext);

	PICK_DESC tPickDesc;
	if (false == XMVector4EqualInt(XMVectorIsNaN(XMLoadFloat4(&tRay.vRayDir)), XMVectorTrueInt()))
	{
		Picked(tPickDesc, tRay);
	}


#ifdef _DEBUG
	// ���콺��ǥ ����ٴ�.
	if (m_bShowBrush)
	{
		m_pRendererCom->Add_DebugGroup(m_pNavigationCom);
	}
#endif // _DEBUG
}

void CTerrain::Late_Tick(_double TimeDelta)
{
	__super::Late_Tick(TimeDelta);
	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this);
}

HRESULT CTerrain::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	if (FAILED(SetUp_ShaderResources()))
		return E_FAIL;

	m_pShaderCom->Begin(0);

	m_pVIBufferCom->Render();

	return S_OK;
}

_bool CTerrain::Picked(PICK_DESC& tPickDesc, const RAY& tMouseRay)
{
	_bool bResult = { false };
	_vector vRayOrigin = XMLoadFloat4(&tMouseRay.vRayOrigin);
	_vector vRayDir = XMLoadFloat4(&tMouseRay.vRayDir);

	_matrix invWorld = m_pTransformCom->Get_WorldMatrix_Inverse();

	vRayOrigin = XMVector3TransformCoord(vRayOrigin, invWorld);
	vRayDir = XMVector3TransformNormal(vRayDir, invWorld);

	vRayDir = XMVector3Normalize(vRayDir);

	_float fMinDist = FLT_MAX;

	// ����Ʈ���� Ž��
	bResult = m_pVIBufferCom->IsPicked(vRayOrigin, vRayDir, fMinDist);
	
	if (bResult)
	{
		_float3 vIntersection;
		XMStoreFloat3(&vIntersection, XMVectorAdd(vRayOrigin, XMVectorScale(vRayDir, fMinDist)));

		_float4x4 worldMatrix = m_pTransformCom->Get_WorldFloat4x4();
		XMStoreFloat3(&vIntersection, XMVector3TransformCoord(XMLoadFloat3(&vIntersection), XMLoadFloat4x4(&worldMatrix)));

		tPickDesc.fDist = fMinDist;
		m_vPickPos = tPickDesc.vPickPos = *(_float4*)&vIntersection;
		tPickDesc.pPickedObject = this;
	}
	
	return bResult;
}


HRESULT CTerrain::Add_Components()
{	
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	/* For.Com_Renderer */
	CRenderer::CRENDERER_DESC tRendererDesc; tRendererDesc.pOwner = this;
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_STATIC, CRenderer::ProtoTag(), L"Com_Renderer", (CComponent**)&m_pRendererCom, &tRendererDesc), E_FAIL);

	CTransform::CTRANSFORM_DESC TransformDesc{7.0, XMConvertToRadians(90.f)}; TransformDesc.pOwner = this;
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_STATIC, CTransform::ProtoTag(), L"Com_Transform", (CComponent**)&m_pTransformCom
		, &TransformDesc), E_FAIL);

	CShader::CSHADER_DESC tShaderDesc; tShaderDesc.pOwner = this;
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_STATIC, L"Prototype_Component_Shader_VtxNorTex", L"Com_Shader", (CComponent**)&m_pShaderCom, &tShaderDesc), E_FAIL);

	CVIBuffer_Terrain::CVIBUFFER_TERRAIN_DESC tVIBufferTerrainDesc; tVIBufferTerrainDesc.pOwner = this;
	FAILED_CHECK_RETURN(__super::Add_Component(pGameInstance->Get_NextLevelIndex(), CVIBuffer_Terrain::ProtoTag(), L"Com_VIBuffer_Terrain", (CComponent**)&m_pVIBufferCom, &tVIBufferTerrainDesc), E_FAIL);

	CTexture::CTEXTURE_DESC tTextureDesc; tTextureDesc.pOwner = this;
	FAILED_CHECK_RETURN(__super::Add_Component(pGameInstance->Get_NextLevelIndex(), L"Prototype_Component_Texture_Terrain", L"Com_Texture", (CComponent**)&m_pTextureCom[TYPE_DIFFUSE], &tTextureDesc), E_FAIL);

#ifdef _DEBUG
	//FAILED_CHECK_RETURN(__super::Add_Component(pGameInstance->Get_NextLevelIndex(), L"Prototype_Component_Texture_Terrain_Mask", L"Com_Texture_Mask", (CComponent**)&m_pTextureCom[TYPE_MASK], &tTextureDesc), E_FAIL);
	FAILED_CHECK_RETURN(__super::Add_Component(pGameInstance->Get_NextLevelIndex(), L"Prototype_Component_Texture_Terrain_Brush", L"Com_Texture_Brush", (CComponent**)&m_pTextureCom[TYPE_BRUSH], &tTextureDesc), E_FAIL);
#endif

	CNavigation::CNAVIGATION_DESC tNavigationDesc; tNavigationDesc.iCurrentIndex = -1, tNavigationDesc.pOwner = this;
	FAILED_CHECK_RETURN(__super::Add_Component(pGameInstance->Get_NextLevelIndex(), CNavigation::ProtoTag(), L"Com_Navigation", (CComponent**)&m_pNavigationCom, &tNavigationDesc), E_FAIL);

	Safe_Release(pGameInstance);
	return S_OK;
}

HRESULT CTerrain::SetUp_ShaderResources()
{
	_float4x4 tmp = m_pTransformCom->Get_WorldFloat4x4();
	FAILED_CHECK_RETURN(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &tmp), E_FAIL);

	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	tmp = pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_VIEW);
	FAILED_CHECK_RETURN(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &tmp), E_FAIL);


	tmp = pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_PROJ);
	FAILED_CHECK_RETURN(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &tmp), E_FAIL);


	FAILED_CHECK_RETURN(m_pTextureCom[TYPE_DIFFUSE]->Bind_ShaderResources(m_pShaderCom, "g_DiffuseTexture"), E_FAIL);
	//FAILED_CHECK_RETURN(m_pTextureCom[TYPE_MASK]->Bind_ShaderResources(m_pShaderCom, "g_MaskTexture"), E_FAIL);
#ifdef _DEBUG
	FAILED_CHECK_RETURN(m_pTextureCom[TYPE_BRUSH]->Bind_ShaderResources(m_pShaderCom, "g_BrushTexture"), E_FAIL);
#endif

	Safe_Release(pGameInstance);

	FAILED_CHECK_RETURN(m_pTextureCom[TYPE_DIFFUSE]->Bind_ShaderResources(m_pShaderCom, "g_DiffuseTexture"), E_FAIL);

	_bool bRed = true;
	FAILED_CHECK_RETURN(m_pShaderCom->Bind_RawValue("g_bRed", &bRed, sizeof(_bool)), E_FAIL);

#ifdef _DEBUG
	if(m_bShowBrush)
		FAILED_CHECK_RETURN(m_pShaderCom->Bind_RawValue("g_vBrushPos", &m_vPickPos, sizeof(_float3)), E_FAIL);
		FAILED_CHECK_RETURN(m_pShaderCom->Bind_RawValue("g_bShowBrush", &m_bShowBrush, sizeof(_bool)), E_FAIL);
#endif
	return S_OK;
}

CTerrain* CTerrain::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CTerrain* pInstance = new CTerrain(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created CTerrain");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CTerrain::Clone(void* pArg)
{
	CTerrain* pInstance = new CTerrain(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned CTerrain");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CTerrain::Free(void)
{
	__super::Free();
	--CTerrain_Id;
	Safe_Release(m_pNavigationCom);

	for (size_t i = 0; i < TYPE_END; i++)
		Safe_Release(m_pTextureCom[i]);

	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pShaderCom);
}

void CTerrain::Save(HANDLE hFile, DWORD& dwByte)
{
	m_tInfo.Save(hFile, dwByte);
	m_pTransformCom->Save(hFile, dwByte);
}

void CTerrain::Load(HANDLE hFile, DWORD& dwByte, _uint iLevelIndex)
{
	m_pTransformCom->Load(hFile, dwByte, iLevelIndex);
}


