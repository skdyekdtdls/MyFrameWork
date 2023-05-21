#include "Renderer.h"
#include "GameObject.h"

CRenderer::CRenderer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CComponent(pDevice, pContext)
{
}

CRenderer::CRenderer(const CRenderer& rhs)
	: CComponent(rhs)
{
}

HRESULT CRenderer::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

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
	HRESULT hr = S_OK;
	for (auto& pGameObject : m_RenderObjects[RENDER_NONBLEND])
	{
		hr = pGameObject->Render();

		Safe_Release(pGameObject);
	}
	m_RenderObjects[RENDER_NONBLEND].clear();

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
	for (auto& RenderList : m_RenderObjects)
	{
		for (auto& pGameObject : RenderList)
			Safe_Release(pGameObject);

		RenderList.clear();
	}
}