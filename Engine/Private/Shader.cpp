#include "..\Public\Shader.h"

CShader::CShader(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CComponent(pDevice, pContext)
{
}

CShader::CShader(const CShader& rhs)
	: CComponent(rhs)
	, m_pEffect(rhs.m_pEffect)
	, m_iNumPasses(rhs.m_iNumPasses)
	, m_InputLayouts(rhs.m_InputLayouts)
{
	for (auto& pInputLayout : m_InputLayouts)
		Safe_AddRef(pInputLayout);

	Safe_AddRef(m_pEffect);
}

HRESULT CShader::Initialize_Prototype(const _tchar* pShaderFilePath
	, const D3D11_INPUT_ELEMENT_DESC* pElements, _uint iNumElements)
{
	_uint iHlslFlag = { 0 };

#ifdef _DEBUG
	iHlslFlag = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#else
	iHlslFlag = D3DCOMPILE_OPTIMIZATION_LEVEL1;
#endif

	if (FAILED(D3DX11CompileEffectFromFile(pShaderFilePath, nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE
		, iHlslFlag, 0, m_pDevice, &m_pEffect, nullptr)))
	{
		assert(false);
		return E_FAIL;
	}

	ID3DX11EffectTechnique* pTechnique = m_pEffect->GetTechniqueByIndex(0);
	if (nullptr == pTechnique)
	{
		assert(false);
		return E_FAIL;
	}
	D3DX11_TECHNIQUE_DESC TechniqueDesc;
	ZeroStruct(TechniqueDesc);
	pTechnique->GetDesc(&TechniqueDesc);
	m_iNumPasses = TechniqueDesc.Passes;

	for (_uint i = 0; i < m_iNumPasses; ++i)
	{
		ID3DX11EffectPass* pPass = pTechnique->GetPassByIndex(i);
		D3DX11_PASS_DESC	PassDesc;
		ZeroStruct(PassDesc);
		pPass->GetDesc(&PassDesc);

		ID3D11InputLayout* pInputLayout = { nullptr };
		if (FAILED(m_pDevice->CreateInputLayout(pElements, iNumElements, PassDesc.pIAInputSignature
			, PassDesc.IAInputSignatureSize, &pInputLayout)))
		{
			assert(false);
			return E_FAIL;
		}

		m_InputLayouts.push_back(pInputLayout);
	}

	return S_OK;
}

HRESULT CShader::Initilize(void* pArg)
{
	return S_OK;
}

HRESULT CShader::Begin(_uint iPassIndex)
{
	if (iPassIndex >= m_iNumPasses)
	{
		CONSOLE_MSG("CShader::Begin \t줄 번호 : " << __LINE__);
		return E_FAIL;
	}

	ID3DX11EffectTechnique* pTechnique = m_pEffect->GetTechniqueByIndex(0);
	if (nullptr == pTechnique)
	{
		CONSOLE_MSG("CShader::Begin \t줄 번호 : " << __LINE__);
		return E_FAIL;
	}

	pTechnique->GetPassByIndex(iPassIndex)->Apply(0, m_pContext);

	m_pContext->IASetInputLayout(m_InputLayouts[iPassIndex]);

	return S_OK;
}

HRESULT CShader::Bind_RawValue(const _char* pConstantName, const void* pValue, _uint iLength)
{
	if (nullptr == m_pEffect)
		return E_FAIL;

	//g_Texture
	ID3DX11EffectVariable* pVariable = m_pEffect->GetVariableByName(pConstantName);
	if (nullptr == pVariable)
		return E_FAIL;

	return pVariable->SetRawValue(pValue, 0, iLength);
}

HRESULT CShader::Bind_ShaderResource(const _char* pConstantName, ID3D11ShaderResourceView* pSRV)
{
	if (nullptr == m_pEffect)
		return E_FAIL;
	
	//g_Texture
	ID3DX11EffectVariable* pVariable = m_pEffect->GetVariableByName(pConstantName);
	if (nullptr == pVariable)
		return E_FAIL;
	//(SR)g_Texture
	ID3DX11EffectShaderResourceVariable* pVarialbeShaderResource = pVariable->AsShaderResource();
	if (nullptr == pVarialbeShaderResource)
		return E_FAIL;

	return pVarialbeShaderResource->SetResource(pSRV);
}

HRESULT CShader::Bind_ShaderResources(const _char* pConstantName, ID3D11ShaderResourceView** ppSRVArray, _uint iNumTexture)
{
 	if (nullptr == m_pEffect)
		return E_FAIL;

	ID3DX11EffectVariable* pVariable = m_pEffect->GetVariableByName(pConstantName);
	if (nullptr == pVariable)
		return E_FAIL;

	ID3DX11EffectShaderResourceVariable* pShaderResourceVariable = pVariable->AsShaderResource();
	if (nullptr == pShaderResourceVariable)
		return E_FAIL;

	return pShaderResourceVariable->SetResourceArray(ppSRVArray, 0, iNumTexture);
}

HRESULT CShader::Bind_Matrix(const _char* pConstantName, const _float4x4* pMatrix)
{
	if (nullptr == m_pEffect)
		return E_FAIL;

	/* 해당하는 이름의 전역변수에 해당하는 컴객체를 얻어오낟. */
	ID3DX11EffectVariable* pVariable = m_pEffect->GetVariableByName(pConstantName);
	if (nullptr == pVariable)
		return E_FAIL;

	ID3DX11EffectMatrixVariable* pVariableMatrix = pVariable->AsMatrix();
	if (nullptr == pVariableMatrix)
		return E_FAIL;

	// 디버깅용
	//pVariableMatrix->SetMatrix((_float*)pMatrix);
	//_float4x4 tmp;
	//pVariableMatrix->GetMatrix((_float*)&tmp);

	/* 해당 컴객체로 변수에 값을 던진다. */
	return pVariableMatrix->SetMatrix((_float*)pMatrix);
}

HRESULT CShader::Bind_Matrices(const _char* pConstantName, const _float4x4* pMatrix, _uint iNumMatrices)
{
	if (nullptr == m_pEffect)
		return E_FAIL;

	/* 해당하는 이름의 전역변수에 해당하는 컴객체를 얻어오낟. */
	ID3DX11EffectVariable* pVariable = m_pEffect->GetVariableByName(pConstantName);
	if (nullptr == pVariable)
		return E_FAIL;

	ID3DX11EffectMatrixVariable* pVariableMatrix = pVariable->AsMatrix();
	if (nullptr == pVariableMatrix)
		return E_FAIL;

	/* 해당 컴객체로 변수에 값을 던진다. */
	return pVariableMatrix->SetMatrixArray((_float*)pMatrix, 0, iNumMatrices);
}

CShader* CShader::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _tchar* pShaderFilePath, const D3D11_INPUT_ELEMENT_DESC* pElements, _uint iNumElements)
{
	CShader* pInstance = new CShader(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(pShaderFilePath, pElements, iNumElements)))
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
	for (auto& pInputLayout : m_InputLayouts)
		Safe_Release(pInputLayout);
	Safe_Release(m_pEffect);
}
