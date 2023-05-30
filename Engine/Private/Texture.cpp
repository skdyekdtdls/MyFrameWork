#include "..\Public\Texture.h"
#include "Shader.h"
CTexture::CTexture(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CComponent(pDevice, pContext)
{
}

CTexture::CTexture(const CTexture& rhs)
	: CComponent(rhs)
	, m_iNumTextures(rhs.m_iNumTextures)
	, m_Textures(rhs.m_Textures)
{
	_ulong dwRefCnt;
	for (_uint i = 0; i < m_iNumTextures; ++i)
	{
		dwRefCnt = Safe_AddRef(m_Textures[i]);
	}
}

HRESULT CTexture::Initialize_Prototype(const _tchar* pTextureFilePath, _uint iNumTextures)
{
	CoInitializeEx(nullptr, 0);

	_tchar szTextureFilePath[MAX_PATH] = TEXT("");
	m_iNumTextures = iNumTextures;
	m_Textures.reserve(m_iNumTextures);
	
	for (_uint i = 0; i < m_iNumTextures; ++i)
	{
		ID3D11ShaderResourceView* pSRV = { nullptr };

		wsprintf(szTextureFilePath, pTextureFilePath, i);

		_tchar szExt[MAX_PATH] = TEXT("");

		_wsplitpath_s(szTextureFilePath, nullptr, 0, nullptr, 0, nullptr, 0, szExt, MAX_PATH);

		HRESULT hr = { 0 };

		if (0 == lstrcmp(szExt, L".dds"))
		{
			hr = DirectX::CreateDDSTextureFromFile(m_pDevice, szTextureFilePath, nullptr, &pSRV);
		}
		else if (0 == lstrcmp(szExt, L".tga"))
		{
			CONSOLE_MSG("CTexture::Initialize_Prototype, .tga예외처리. 줄 번호 : " << __LINE__);
		}
		else
		{
			hr = DirectX::CreateWICTextureFromFile(m_pDevice, szTextureFilePath, nullptr, &pSRV);
		}

		if (FAILED(hr))
		{
			MSG_BOX("Failed to Load CTexture::Initialize_Prototype");
			return E_FAIL;
		}
		
		m_Textures.push_back(pSRV);
	}
	return S_OK;
}

HRESULT CTexture::Initialize(void* pArg)
{
	return S_OK;
}

HRESULT CTexture::Bind_ShaderResource(CShader* pShader, const _char* pConstantName, _uint iTextureIndex)
{
	if (iTextureIndex >= m_iNumTextures)
		return E_FAIL;

	return pShader->Bind_ShaderResource(pConstantName, m_Textures[iTextureIndex]);
}

HRESULT CTexture::Bind_ShaderResources(CShader* pShader, const _char* pConstantName)
{
	return pShader->Bind_ShaderResources(pConstantName, &m_Textures.front(), m_iNumTextures);
}

CTexture* CTexture::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext
	, const _tchar* pTextureFilePath, _uint iNumTextures)
{
	CTexture* pInstance = new CTexture(pDevice, pContext);
	if (FAILED(pInstance->Initialize_Prototype(pTextureFilePath, iNumTextures)))
	{
		MSG_BOX("Failed to Created CTexture");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CComponent* CTexture::Clone(void* pArg)
{
	CTexture* pInstance = new CTexture(*this);
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned CTexture");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CTexture::Free(void)
{
	__super::Free();
	
	for (_uint i = 0; i < m_iNumTextures; ++i)
	{
		Safe_Release(m_Textures[i]);
	}
}
