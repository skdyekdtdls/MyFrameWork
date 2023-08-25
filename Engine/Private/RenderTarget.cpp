#include "..\Public\RenderTarget.h"
#include "Shader.h"

#ifdef _DEBUG
#include "VIBuffer_Rect.h"
#endif // _DEBUG

CRenderTarget::CRenderTarget(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: m_pDevice(pDevice)
	, m_pContext(pContext)
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

HRESULT CRenderTarget::Initialize(_uint iSizeX, _uint iSizeY, DXGI_FORMAT eFormat, const _float4& vClearColor)
{
	D3D11_TEXTURE2D_DESC TextureDesc;
	ZeroStruct(TextureDesc);

	TextureDesc.Width = iSizeX;
	TextureDesc.Height = iSizeY;
	TextureDesc.MipLevels = 1;
	TextureDesc.ArraySize = 1;
	TextureDesc.Format = eFormat;

	TextureDesc.SampleDesc.Quality = 0;
	TextureDesc.SampleDesc.Count = 1;

	TextureDesc.Usage = D3D11_USAGE_DEFAULT;
	TextureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	TextureDesc.CPUAccessFlags = 0;
	TextureDesc.MiscFlags = 0;

	FAILED_CHECK_RETURN(m_pDevice->CreateTexture2D(&TextureDesc, nullptr, &m_pTexture2D), E_FAIL);
	FAILED_CHECK_RETURN(m_pDevice->CreateRenderTargetView(m_pTexture2D, nullptr, &m_pRTV), E_FAIL);
	FAILED_CHECK_RETURN(m_pDevice->CreateShaderResourceView(m_pTexture2D, nullptr, &m_pSRV), E_FAIL);

	m_vClearColor = vClearColor;

	return S_OK;
}

#ifdef _DEBUG	
HRESULT CRenderTarget::Ready_Debug(_float fX, _float fY, _float fSizeX, _float fSizeY)
{
	XMStoreFloat4x4(&m_WorldMatrix, XMMatrixIdentity());

	_uint iNumViews = { 1 };
	D3D11_VIEWPORT ViewportDesc;

	m_pContext->RSGetViewports(&iNumViews, &ViewportDesc);

	m_WorldMatrix._11 = fSizeX;
	m_WorldMatrix._22 = fSizeY;

	m_WorldMatrix._41 = fX - ViewportDesc.Width * 0.5f;
	m_WorldMatrix._42 = -fY + ViewportDesc.Height * 0.5f;

	return S_OK;
}
HRESULT CRenderTarget::Render(CShader* pShader, CVIBuffer_Rect* pVIBuffer)
{
	FAILED_CHECK_RETURN(pShader->Bind_Matrix("g_WorldMatrix", &m_WorldMatrix), E_FAIL);
	FAILED_CHECK_RETURN(pShader->Bind_ShaderResource("g_Texture", m_pSRV), E_FAIL);
	FAILED_CHECK_RETURN(pShader->Begin(0), E_FAIL);
	
	return pVIBuffer->Render();
}
#endif

HRESULT CRenderTarget::Clear()
{
	m_pContext->ClearRenderTargetView(m_pRTV, (_float*)&m_vClearColor);

	return S_OK;
}

HRESULT CRenderTarget::Bind_ShaderResourceView(CShader* pShader, const char* pConstantName)
{
	if (nullptr == m_pSRV)
		return E_FAIL;

	return pShader->Bind_ShaderResource(pConstantName, m_pSRV);
}

CRenderTarget* CRenderTarget::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, _uint iSizeX, _uint iSizeY, DXGI_FORMAT eFormat, const _float4& vClearColor)
{
	CRenderTarget* pInstance = new CRenderTarget(pDevice, pContext);

	if (FAILED(pInstance->Initialize(iSizeX, iSizeY, eFormat, vClearColor)))
	{
		MSG_BOX("Failed to Created CRenderTarget");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CRenderTarget::Free()
{
	Safe_Release(m_pRTV);
	Safe_Release(m_pSRV);
	Safe_Release(m_pTexture2D);
	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
}