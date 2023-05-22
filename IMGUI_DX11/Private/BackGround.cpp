#include "..\Public\BackGround.h"
#include "GameInstance.h"
#include "Shader.h"
#include "VIBuffer_Rect.h"

CBackGround::CBackGround(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject(pDevice, pContext)
{
}

CBackGround::CBackGround(const CBackGround& rhs)
	: CGameObject(rhs)
{
	
}

HRESULT CBackGround::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CBackGround::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Add_Components()))
		return E_FAIL;

	return S_OK;
}

void CBackGround::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);

	
}

void CBackGround::Late_Tick(_double TimeDelta)
{
	__super::Late_Tick(TimeDelta);
	if(nullptr != m_pRendererCom)
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_PRIORITY, this);
}

HRESULT CBackGround::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	if (FAILED(SetUp_ShaderResources()))
		return E_FAIL;
	
	if (FAILED(m_pShaderCom->Begin(0)))
	{
		assert(false);
		return E_FAIL;
	}

	if (FAILED(m_pViBufferCom->Render()))
	{
		assert(false);
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CBackGround::SetUp_ShaderResources()
{
	return S_OK;
}

HRESULT CBackGround::Add_Components()
{
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer")
		, TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
	{
		assert(false);
		return E_FAIL;
	}

	/* For.Com_VIBuffer_Rect */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect")
		, TEXT("Com_VIBuffer"), (CComponent**)&m_pViBufferCom)))
	{
		assert(false);
		return E_FAIL;
	}

	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_Vtxtex")
		, TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
	{
		assert(false);
		return E_FAIL;
	}

	return S_OK;
}

CBackGround* CBackGround::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CBackGround* pInstance = new CBackGround(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created CBackGround");
		Safe_Release(pInstance);
	}
	return pInstance;
}


CGameObject* CBackGround::Clone(void* pArg)
{
	CBackGround* pInstance = new CBackGround(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned CBackGround");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CBackGround::Free()
{
	__super::Free();

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pViBufferCom);
	Safe_Release(m_pRendererCom); 
}
