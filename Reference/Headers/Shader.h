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
	virtual HRESULT Initialize_Prototype(const _tchar * pShaderFilePath);
	virtual HRESULT Initilize(void* pArg);

public:
	HRESULT Begin();

private:
	ID3DX11Effect* m_pEffect = { nullptr };
	ID3D11InputLayout* m_pInputLayout = { nullptr };

public:
	static CShader* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, const _tchar * pShaderFilePath);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free(void) override;
};
END
