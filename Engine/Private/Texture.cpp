#include "..\Public\Texture.h"
#include "Shader.h"
CTexture::CTexture(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CComponent(pDevice, pContext)
{
}

CTexture::CTexture(const CTexture& rhs)
	: CComponent(rhs)
	, m_iNumTextures(rhs.m_iNumTextures)
	, m_ppTextures(rhs.m_ppTextures)
{
	for (_uint i = 0; i < m_iNumTextures; ++i)
	{
		Safe_AddRef(m_ppTextures[i]);
	}
}

HRESULT CTexture::Initialize_Prototype(const _tchar* pTextureFilePath, _uint iNumTextures)
{
	_tchar szTextureFilePath[MAX_PATH] = TEXT("");
	m_iNumTextures = iNumTextures;
	m_ppTextures = new ID3D11ShaderResourceView * [m_iNumTextures];
	
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
			CONSOLE_MSG("CTexture::Initialize_Prototype, .tga����ó��. �� ��ȣ : " << __LINE__);
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
		
		m_ppTextures[i] = pSRV;
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

	return pShader->Bind_ShaderResource(pConstantName, m_ppTextures[iTextureIndex]);
}

HRESULT CTexture::Bind_ShaderResources(CShader* pShader, const _char* pConstantName)
{
	return pShader->Bind_ShaderResources(pConstantName, m_ppTextures, m_iNumTextures);
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
	
	_long dwRefCnt = 0;
	for (_uint i = 0; i < m_iNumTextures; ++i)
	{
		dwRefCnt = Safe_Release(m_ppTextures[i]);
	}

	if(0 == dwRefCnt)
		Safe_Delete_Array(m_ppTextures);
}