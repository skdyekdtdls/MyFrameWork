#include "Renderer.h"
#include "GameObject.h"
#include "Target_Manager.h"
#include "Light_Manager.h"

#include "Shader.h"
#include "PipeLine.h"
#include "VIBuffer_Rect.h"

CRenderer::CRenderer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CComponent(pDevice, pContext)
	, m_pTarget_Manager(CTarget_Manager::GetInstance())
	, m_pLight_Manager(CLight_Manager::GetInstance())
{
	Safe_AddRef(m_pTarget_Manager);
	Safe_AddRef(m_pLight_Manager);
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
		TEXT("Target_Diffuse"), ViewPortDesc.Width, ViewPortDesc.Height, DXGI_FORMAT_B8G8R8A8_UNORM, _float4(1.f, 1.f, 1.f, 0.f))))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext,
		TEXT("Target_Normal"), ViewPortDesc.Width, ViewPortDesc.Height, DXGI_FORMAT_B8G8R8A8_UNORM, _float4(1.f, 1.f, 1.f, 1.f))))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext,
		TEXT("Target_Depth"), ViewPortDesc.Width, ViewPortDesc.Height, DXGI_FORMAT_R32G32B32A32_FLOAT, _float4(0.f, 0.f, 0.f, 1.f))))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext,
		TEXT("Target_Shade"), ViewPortDesc.Width, ViewPortDesc.Height, DXGI_FORMAT_B8G8R8A8_UNORM, _float4(1.f, 1.f, 1.f, 1.f))))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext,
		TEXT("Target_Specular"), ViewPortDesc.Width, ViewPortDesc.Height, DXGI_FORMAT_B8G8R8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;

	// 멀티렌더타겟 추가
	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_GameObjects"), TEXT("Target_Diffuse"))))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_GameObjects"), TEXT("Target_Normal"))))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_GameObjects"), TEXT("Target_Depth"))))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_Lights"), TEXT("Target_Shade"))))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_Lights"), TEXT("Target_Specular"))))
		return E_FAIL;

	m_pShader = CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_Deferred.hlsl")
		, VTXPOSTEX_DECL::Elements, VTXPOSTEX_DECL::iNumElements);
	NULL_CHECK_RETURN(m_pShader, E_FAIL);

	m_pVIBuffer = CVIBuffer_Rect::Create(m_pDevice, m_pContext);
	NULL_CHECK_RETURN(m_pVIBuffer, E_FAIL);

	XMStoreFloat4x4(&m_WorldMatrix, XMMatrixIdentity());
	m_WorldMatrix._11 = ViewPortDesc.Width;
	m_WorldMatrix._22 = ViewPortDesc.Height;
	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());
	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH(ViewPortDesc.Width, ViewPortDesc.Height, 0.f, 1.f));

#ifdef _DEBUG
	m_pTarget_Manager->Ready_Debug(TEXT("Target_Diffuse"), 100.f, 100.f, 200.f, 200.f);
	m_pTarget_Manager->Ready_Debug(TEXT("Target_Normal"), 100.f, 300.f, 200.f, 200.f);
	m_pTarget_Manager->Ready_Debug(TEXT("Target_Depth"), 100.f, 500.f, 200.f, 200.f);
	m_pTarget_Manager->Ready_Debug(TEXT("Target_Shade"), 300.f, 100.f, 200.f, 200.f);
	m_pTarget_Manager->Ready_Debug(TEXT("Target_Specular"), 300.f, 300.f, 200.f, 200.f);
#endif // _DEBUG
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

	if (FAILED(Render_Lights()))
	{
		CONSOLE_MSG("CRenderer::Draw_RenderGroup : \t 줄 번호 : " << __LINE__);
		return E_FAIL;
	}

	if (FAILED(Render_Deferred()))
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

#ifdef _DEBUG
	if (FAILED(Render_Debug()))
	{
		CONSOLE_MSG("CRenderer::Draw_RenderGroup : \t 줄 번호 : " << __LINE__);
		return E_FAIL;
	}
#endif // _DEBUG

	if (FAILED(Render_UI()))
	{
		CONSOLE_MSG("CRenderer::Draw_RenderGroup : \t 줄 번호 : " << __LINE__);
		return E_FAIL;
	}

	if (FAILED(Render_Font()))
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

HRESULT CRenderer::Render_Lights()
{
	if (nullptr == m_pTarget_Manager)
		return E_FAIL;

	/* Shade(0) + Specular(1) */
	if (FAILED(m_pTarget_Manager->Begin_MRT(m_pContext, TEXT("MRT_Lights"))))
		return E_FAIL;

	if (FAILED(m_pShader->Bind_Matrix("g_WorldMatrix", &m_WorldMatrix)))
		return E_FAIL;
	if (FAILED(m_pShader->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShader->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	CPipeLine* pPipeLine = CPipeLine::GetInstance();
	Safe_AddRef(pPipeLine);

	_float4x4 matViewInv = pPipeLine->Get_TransformFloat4x4_Inverse(CPipeLine::D3DTS_VIEW);
	if (FAILED(m_pShader->Bind_Matrix("g_ViewMatrixInv",
		&matViewInv)))
		return E_FAIL;

	_float4x4 matProjInv = pPipeLine->Get_TransformFloat4x4_Inverse(CPipeLine::D3DTS_PROJ);
	if (FAILED(m_pShader->Bind_Matrix("g_ProjMatrixInv",
		&matProjInv)))
		return E_FAIL;

	_float4 CamPos = pPipeLine->Get_CamPositionFloat4();
	if (FAILED(m_pShader->Bind_RawValue("g_vCamPosition", &CamPos, sizeof(_float4))))
		return E_FAIL;

	Safe_Release(pPipeLine);

	/* 노멀 렌더타겟을 셰이더 전역으로 던진다.*/
	if (FAILED(m_pTarget_Manager->Bind_ShaderResourceView(TEXT("Target_Normal"), m_pShader, "g_NormalTexture")))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Bind_ShaderResourceView(TEXT("Target_Depth"), m_pShader, "g_DepthTexture")))
		return E_FAIL;

	/* 빛 정보를 던진다. */
	m_pLight_Manager->Render_Lights(m_pShader, m_pVIBuffer);

	if (FAILED(m_pTarget_Manager->End_MRT(m_pContext)))
		return E_FAIL;

	return S_OK;
}

HRESULT CRenderer::Render_Deferred()
{
	// 백버퍼에 최종적으로 그려낸다.

	// 디퓨즈 렌더타겟을 셰이더 전역으로 던진다.
	if (FAILED(m_pTarget_Manager->Bind_ShaderResourceView(TEXT("Target_Diffuse"), m_pShader, "g_DiffuseTexture")))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Bind_ShaderResourceView(TEXT("Target_Shade"), m_pShader, "g_ShadeTexture")))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Bind_ShaderResourceView(TEXT("Target_Specular"), m_pShader, "g_SpecularTexture")))
		return E_FAIL;

	if (FAILED(m_pShader->Bind_Matrix("g_WorldMatrix", &m_WorldMatrix)))
		return E_FAIL;
	if (FAILED(m_pShader->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShader->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	m_pShader->Begin(3);

	m_pVIBuffer->Render();

	return S_OK;
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

HRESULT CRenderer::Render_Font()
{
	HRESULT hr = S_OK;
	for (auto& pGameObject : m_RenderObjects[RENDER_FONT])
	{
		hr = pGameObject->Render();

		Safe_Release(pGameObject);
	}
	m_RenderObjects[RENDER_FONT].clear();

	return hr;
}

#ifdef _DEBUG
HRESULT CRenderer::Add_DebugGroup(CComponent* pDebugCom)
{
	m_DebugObject.push_back(pDebugCom);

	Safe_AddRef(pDebugCom);

	return S_OK;
}

HRESULT CRenderer::Render_Debug()
{
	if (nullptr == m_pTarget_Manager)
		return E_FAIL;

	for (auto& pDebugCom : m_DebugObject)
	{
		if (nullptr != pDebugCom)
			pDebugCom->Render();

		Safe_Release(pDebugCom);
	}
	m_DebugObject.clear();

	if (FAILED(m_pShader->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;

	if (FAILED(m_pShader->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Render_Debug(TEXT("MRT_GameObjects"), m_pShader, m_pVIBuffer)))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Render_Debug(TEXT("MRT_Lights"), m_pShader, m_pVIBuffer)))
		return E_FAIL;

	return S_OK;
}
#endif // _DEBUG


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
	Safe_Release(m_pTarget_Manager);
	Safe_Release(m_pLight_Manager);

	Safe_Release(m_pVIBuffer);
	Safe_Release(m_pShader);
}
