#pragma once

#include "Component.h"

BEGIN(Engine)
class CShader;
class ENGINE_DLL CTexture final : public CComponent
{
private:
	CTexture(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CTexture(const CTexture& rhs);
	virtual ~CTexture() = default;

public:
	HRESULT Initialize_Prototype(const _tchar * pTextureFilePath, _uint iNumTextures);
	virtual HRESULT Initialize(void* pArg);

public:
	HRESULT Bind_ShaderResource(CShader * pShader, const _char * pConstantName, _uint iTextureIndex = 0);
	HRESULT Bind_ShaderResources(CShader * pShader, const _char * pConstantName);

private:
	_uint	m_iNumTextures = { 0 };
	vector<ID3D11ShaderResourceView*> m_Textures;

public:
	static CTexture* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, const _tchar * pTextureFilePath, _uint iNumTextures = 1);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free(void) override;
};
END