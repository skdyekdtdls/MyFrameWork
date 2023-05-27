#pragma once

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CShader final : public CComponent
{
protected:
	CShader(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CShader(const CShader& rhs);
	virtual ~CShader() = default;

public:
	virtual HRESULT Initialize_Prototype(const _tchar * pShaderFilePath
		, const D3D11_INPUT_ELEMENT_DESC * pElements, _uint iNumElements);
	virtual HRESULT Initilize(void* pArg);

public:
	HRESULT Begin(_uint iPassIndex);

	HRESULT Bind_ShaderResource(const _char * pConstantName, ID3D11ShaderResourceView * pSRV);
	HRESULT Bind_ShaderResources(const _char * pConstantName, ID3D11ShaderResourceView * *ppSRVArray, _uint iNumTexture);
	HRESULT Bind_Matrix(const _char * pConstantName, const _float4x4 * pMatrix);
	
private:
	_uint	m_iNumPasses = { 0 };
	vector<ID3D11InputLayout*>	m_InputLayouts;
	ID3DX11Effect* m_pEffect = { nullptr };

public:
	static CShader* Create(ID3D11Device * pDevice
		, ID3D11DeviceContext * pContext
		, const _tchar * pShaderFilePath
		, const D3D11_INPUT_ELEMENT_DESC * pElements
		, _uint iNumElements);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free(void) override;
};
END
