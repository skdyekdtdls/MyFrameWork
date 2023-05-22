#include "..\Public\Terrain.h"
#include "GameInstance.h"
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
	return S_OK;
}

HRESULT CTerrain::Initialize(void* pArg)
{
	return S_OK;
}

void CTerrain::Tick(_double TimeDelta)
{
}

void CTerrain::Late_Tick(_double TimeDelta)
{
	pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this);
}

HRESULT CTerrain::Render()
{
	return S_OK;
}

HRESULT CTerrain::Add_Components()
{
	if(FAILED(__super::Add_Component())

	return S_OK;
}

CTerrain* CTerrain::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	return nullptr;
}

CGameObject* CTerrain::Clone(void* pArg)
{
	return nullptr;
}

void CTerrain::Free(void)
{
	Safe_Release(pRendererCom);
	Safe_Release(pVIBufferCom);
	Safe_Release(pShaderCom);
}
