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
