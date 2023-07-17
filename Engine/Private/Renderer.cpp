#include "Renderer.h"
#include "GameObject.h"
#include "Target_Manager.h"

CRenderer::CRenderer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CComponent(pDevice, pContext)
	, m_pTarget_Manager(CTarget_Manager::GetInstance())
{
	Safe_AddRef(m_pTarget_Manager);
}

HRESULT CRenderer::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	_uint iNumViews = { 1 };
	D3D11_VIEWPORT ViewPortDesc;

	m_pContext->RSGetViewports(&iNumViews, &ViewPortDesc);

	// 렌더타겟 추가
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext,
		TEXT("Target_Diffuse"), ViewPortDesc.Width, ViewPortDesc.Height, DXGI_FORMAT_B8G8R8A8_UNORM, _float4(1.f, 1.f, 1.f, 1.f))))
		return E_FAIL;

	//if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext,
	//	TEXT("Target_Normal"), ViewPortDesc.Width, ViewPortDesc.Height, DXGI_FORMAT_B8G8R8A8_UNORM, _float4(1.f, 1.f, 1.f, 1.f))))
	//	return E_FAIL;

	//if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext,
	//	TEXT("Target_Shade"), ViewPortDesc.Width, ViewPortDesc.Height, DXGI_FORMAT_B8G8R8A8_UNORM, _float4(1.f, 1.f, 1.f, 1.f))))
	//	return E_FAIL;

	// 멀티렌더타겟 추가
	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_GameObjects"), TEXT("Target_Diffuse"))))
		return E_FAIL;

	//if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_GameObjects"), TEXT("Target_Normal"))))
	//	return E_FAIL;

	//if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_Lights"), TEXT("Target_Shade"))))
	//	return E_FAIL;

	return S_OK;
}

HRESULT CRenderer::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

void CRenderer::Add_RenderGroup(RENDERGROUP eRenderGroup, CGameObject* pGameObejct)
{
	if (eRenderGroup >= RENDER_END || nullptr == pGameObejct)
		return;

	Safe_AddRef(pGameObejct);
	m_RenderObjects[eRenderGroup].push_back(pGameObejct);
}

HRESULT CRenderer::Draw_RenderGroup()
{
	if (FAILED(Render_Priority()))
	{
		CONSOLE_MSG("CRenderer::Draw_RenderGroup : \t 줄 번호 : " << __LINE__);
		return E_FAIL;
	}

	if (FAILED(Render_NonBlend()))
	{
		CONSOLE_MSG("CRenderer::Draw_RenderGroup : \t 줄 번호 : " << __LINE__);
		return E_FAIL;
	}

	if (FAILED(Render_NonLight()))
	{
		CONSOLE_MSG("CRenderer::Draw_RenderGroup : \t 줄 번호 : " << __LINE__);
		return E_FAIL;
	}

	if (FAILED(Render_Blend()))
	{
		CONSOLE_MSG("CRenderer::Draw_RenderGroup : \t 줄 번호 : " << __LINE__);
		return E_FAIL;
	}

	if (FAILED(Render_UI()))
	{
		CONSOLE_MSG("CRenderer::Draw_RenderGroup : \t 줄 번호 : " << __LINE__);
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CRenderer::Render_Priority()
{
	HRESULT hr = S_OK;
	for (auto& pGameObject : m_RenderObjects[RENDER_PRIORITY])
	{
		hr= pGameObject->Render();

		Safe_Release(pGameObject);
	}
	m_RenderObjects[RENDER_PRIORITY].clear();
		
	return hr;
}

HRESULT CRenderer::Render_NonBlend()
{
	m_pTarget_Manager->Begin_MRT(m_pContext, TEXT("MRT_GameObjects"));

	HRESULT hr = S_OK;
	for (auto& pGameObject : m_RenderObjects[RENDER_NONBLEND])
	{
		hr = pGameObject->Render();

		Safe_Release(pGameObject);
	}
	m_RenderObjects[RENDER_NONBLEND].clear();

	m_pTarget_Manager->End_MRT(m_pContext);

	return hr;
}

HRESULT CRenderer::Render_NonLight()
{
	HRESULT hr = S_OK;
	for (auto& pGameObject : m_RenderObjects[RENDER_NONLIGHT])
	{
		hr = pGameObject->Render();

		Safe_Release(pGameObject);
	}
	m_RenderObjects[RENDER_NONLIGHT].clear();

	return hr;
}

HRESULT CRenderer::Render_Blend()
{
	HRESULT hr = S_OK;
	for (auto& pGameObject : m_RenderObjects[RENDER_BLEND])
	{
		hr = pGameObject->Render();

		Safe_Release(pGameObject);
	}
	m_RenderObjects[RENDER_BLEND].clear();

	return hr;
}

HRESULT CRenderer::Render_UI()
{
	HRESULT hr = S_OK;
	for (auto& pGameObject : m_RenderObjects[RENDER_UI])
	{
		hr = pGameObject->Render();

		Safe_Release(pGameObject);
	}
	m_RenderObjects[RENDER_UI].clear();

	return hr;
}



CRenderer* CRenderer::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CRenderer* pInstance = new CRenderer(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created CRenderer");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CRenderer::Clone(void* pArg)
{
	AddRef();

	return this;
}

void CRenderer::Free(void)
{
	__super::Free();
	Safe_Release(m_pTarget_Manager);
	for (auto& RenderList : m_RenderObjects)
	{
		for (auto& pGameObject : RenderList)
			Safe_Release(pGameObject);

		RenderList.clear();
	}
}