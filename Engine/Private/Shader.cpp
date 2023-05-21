#include "..\Public\Shader.h"

CShader::CShader(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CComponent(pDevice, pContext)
{
}

CShader::CShader(const CShader& rhs)
	: CComponent(rhs)
	, m_pEffect(rhs.m_pEffect)
{
	Safe_AddRef(m_pEffect);
}

HRESULT CShader::Initialize_Prototype(const _tchar* pShaderFilePath)
{
	_uint iHlslFlag = { 0 };
#ifdef _DEBUG
	iHlslFlag = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#else
	iHlslFlag = D3DCOMPILE_OPTIMIZATION_LEVEL1;
#endif
	if (FAILED(D3DX11CompileEffectFromFile(pShaderFilePath, nullptr
		, D3D_COMPILE_STANDARD_FILE_INCLUDE, iHlslFlag, 0, m_pDevice, &m_pEffect, nullptr)))
	{
		CONSOLE_MSG("CShader::Initialize_Prototype \t �� ��ȣ : " << __LINE__);
		assert(false);
		return E_FAIL;
	}
		
	ID3DX11EffectTechnique* 	pTechnique  = m_pEffect->GetTechniqueByIndex(0);
	if (nullptr == pTechnique)
	{
		CONSOLE_MSG("CShader::Initialize_Prototype \t�� ��ȣ : " << __LINE__);
		assert(false);
		return E_FAIL;
	}

	D3DX11_TECHNIQUE_DESC		TechniqueDesc;
	ZeroMemory(&pTechnique , sizeof D3DX11_TECHNIQUE_DESC);

	pTechnique ->GetDesc(&TechniqueDesc);

	_uint iNumPasses = TechniqueDesc.Passes;
	for (_uint i = 0; i < iNumPasses; ++i)
	{
		ID3DX11EffectPass* pPass =  pTechnique->GetPassByIndex(i);

		D3DX11_PASS_DESC PassDesc;
		ZeroMemory(&PassDesc, sizeof D3DX11_PASS_DESC);
		pPass->GetDesc(&PassDesc);

		//TODO
	}

	return S_OK;
}

HRESULT CShader::Initilize(void* pArg)
{
	return S_OK;
}

HRESULT CShader::Begin()
{
	m_pContext->IASetInputLayout(m_pInputLayout);

	return S_OK;
}

CShader* CShader::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _tchar* pShaderFilePath)
{
	CShader* pInstance = new CShader(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(pShaderFilePath)))
	{
		MSG_BOX("Failed to Created CShader");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CComponent* CShader::Clone(void* pArg)
{
	CShader* pInstance = new CShader(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned CShader");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CShader::Free(void)
{
	__super::Free();
	Safe_Release(m_pEffect);
}
