#pragma once

#include "Component.h"
#include "Model.h"
BEGIN(Engine)
class CShader;
class ENGINE_DLL CTexture final : public CComponent
{
	public:
	typedef struct tagCTextureDesc : public tagComponentDesc
	{
		tagCTextureDesc() : tagComponentDesc() {};
	}CTEXTURE_DESC;
public:
	CTexture(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CTexture(const CTexture& rhs);
	virtual ~CTexture() = default;

public:
	void SaveAssimp(HANDLE hFile, DWORD & dwByte);
	void LoadAssimp(HANDLE hFile, DWORD & dwByte, CModel::TYPE eType);

public:
	HRESULT Initialize_Prototype(const _tchar * pTextureFilePath, _uint iNumTextures);
	virtual HRESULT Initialize(void* pArg);

public:
	HRESULT Bind_ShaderResource(CShader * pShader, const _char * pConstantName, _uint iTextureIndex = 0);
	HRESULT Bind_ShaderResources(CShader * pShader, const _char * pConstantName);

private:
	_uint	m_iNumTextures = { 0 };

	vector<wstring> m_TextureFilePaths;
	vector<ID3D11ShaderResourceView*> m_Textures;

public:
	static CTexture* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, const _tchar * pTextureFilePath, _uint iNumTextures = 1);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free(void) override;
};
END