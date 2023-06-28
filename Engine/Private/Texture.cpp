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

void CTexture::SaveAssimp(HANDLE hFile, DWORD& dwByte)
{
	WriteVoid(&m_iNumTextures, sizeof(_uint));
	for (size_t i = 0; i < m_iNumTextures; ++i)
	{
		DWORD size = m_TextureFilePaths[i].size();
		WriteVoid(&size, sizeof(size));
		WriteVoid(m_TextureFilePaths[i].c_str(), sizeof(_tchar)* size);
		wcout << m_TextureFilePaths[i].c_str() << endl;
	}
		
}

void CTexture::LoadAssimp(HANDLE hFile, DWORD& dwByte, CModel::TYPE eType)
{
	// 원본 경로를 Skeletal_Mesh, Static_Mesh로 나누어서 경로를 가공해준다.
	fs::path baseDir("../../Resources/");
	fs::path subDir = (eType == CModel::TYPE_ANIM) ? "Skeletal_Mesh/" : "Static_Mesh/";
	fs::path fullpath;
	fs::path ParentPathName;

	ReadVoid(&m_iNumTextures, sizeof(_uint));
	m_TextureFilePaths.resize(m_iNumTextures);
	for (size_t i = 0; i < m_iNumTextures; ++i)
	{
		DWORD size = m_TextureFilePaths[i].size();
		ReadVoid(&size, sizeof(size));

		std::vector<_tchar> temp(size + 1); // temporary vector to hold the string data
		ReadVoid(temp.data(), sizeof(_tchar) * size); // read the string data

		temp[size] = '\0'; // ensure the string is null-terminated
		fullpath  = temp.data();
		ParentPathName = fullpath.parent_path().filename();
		baseDir /= subDir;
		baseDir /= ParentPathName;
		baseDir /= fullpath.filename();
		m_TextureFilePaths[i] = baseDir;
	}

	// png같은 파일들을 .dds 파일로 만들어 주는 코드
	_tchar szTextureFilePath[MAX_PATH] = TEXT("");
	m_Textures.reserve(m_iNumTextures);
	for (_uint i = 0; i < m_iNumTextures; ++i)
	{
		ID3D11ShaderResourceView* pSRV = { nullptr };

		wsprintf(szTextureFilePath, m_TextureFilePaths[i].c_str(), i);

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
			return;
		}
		wcout << szTextureFilePath << endl;
		m_Textures.push_back(pSRV);
	}
	m_TextureFilePaths.clear();
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
		m_TextureFilePaths.push_back(szTextureFilePath);
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
