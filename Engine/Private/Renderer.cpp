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
		return E_FAIL;

	if (FAILED(Render_NonBlend()))
		return E_FAIL;

	if (FAILED(Render_NonLight()))
		return E_FAIL;

	if (FAILED(Render_Blend()))
		return E_FAIL;

	if (FAILED(Render_UI()))
		return E_FAIL;

	return S_OK;
}

HRESULT CRenderer::Render_Priority()
{
	for (auto& GameObject : m_RenderObjects[RENDER_PRIORITY])
	{
		if (FAILED(GameObject->Render()))
			return E_FAIL;

		Safe_Release(GameObject);
	}
	m_RenderObjects[RENDER_PRIORITY].clear();
		
	return S_OK;
}

HRESULT CRenderer::Render_NonBlend()
{
	for (auto& GameObject : m_RenderObjects[RENDER_NONBLEND])
	{
		if (FAILED(GameObject->Render()))
			return E_FAIL;

		Safe_Release(GameObject);
	}
	m_RenderObjects[RENDER_NONBLEND].clear();
	return S_OK;
}

HRESULT CRenderer::Render_NonLight()
{
	for (auto& GameObject : m_RenderObjects[RENDER_NONLIGHT])
	{
		if (FAILED(GameObject->Render()))
			return E_FAIL;

		Safe_Release(GameObject);
	}
	m_RenderObjects[RENDER_NONLIGHT].clear();
	return S_OK;
}

HRESULT CRenderer::Render_Blend()
{
	for (auto& GameObject : m_RenderObjects[RENDER_BLEND])
	{
		if (FAILED(GameObject->Render()))
			return E_FAIL;

		Safe_Release(GameObject);
	}
	m_RenderObjects[RENDER_BLEND].clear();
	return S_OK;
}

HRESULT CRenderer::Render_UI()
{
	for (auto& GameObject : m_RenderObjects[RENDER_UI])
	{
		if (FAILED(GameObject->Render()))
			return E_FAIL;

		Safe_Release(GameObject);
	}
	m_RenderObjects[RENDER_UI].clear();
	return S_OK;
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