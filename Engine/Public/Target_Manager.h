#pragma once

#include "Base.h"

// ����Ÿ���� ������ �����Ͽ� �����ϱ� ���ؼ�

BEGIN(Engine)

class CTarget_Manager final : public CBase
{
	DECLARE_SINGLETON(CTarget_Manager)
private:
	CTarget_Manager() = default;
	virtual ~CTarget_Manager() = default;

public:
	// ����Ÿ�� ���� �� �߰�
	HRESULT Add_RenderTarget(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _tchar* pTargetTag, _uint iSizeX, _uint iSizeY, DXGI_FORMAT eFormat, const _float4& vClearColor);;
	
	// ����Ÿ���� �˻��Ͽ� ��Ƽ����Ÿ�� �׷쿡 �ִ´�.(MRT�� ������ ���� ������.)
	HRESULT Add_MRT(const _tchar* pMRTTag, const _tchar* pTargetTag);

	/* pMRTTag�� �߰��Ǿ��ִ� ����Ÿ�ٵ��� ��ġ�� ���ε��Ѵ�. */
	HRESULT Begin_MRT(ID3D11DeviceContext* pContext, const _tchar* pMRTTag);
	HRESULT End_MRT(ID3D11DeviceContext* pContext);

	/* �������·� �����Ѵ�. (0��°�� ����۰� ���ε� �� ���·� �����ش�. */
	HRESULT Begin_PreMRT(ID3D11DeviceContext* pContext, const _tchar* pMRTTag);
	HRESULT End_PreMRT(ID3D11DeviceContext* pContext);

	// ����Ÿ���� �˻� �� ���ڷ� ���� ���̴��� ������ ���ε��Ѵ�.
	HRESULT Bind_ShaderResourceView(const _tchar* pTargetTag, class CShader* pShader, const char* pConstantName);

#ifdef _DEBUG
public:
	// ����Ÿ���� �˻��ؼ� Ready_Debug�Լ��� ȣ���Ѵ�.
	HRESULT Ready_Debug(const _tchar* pTargetTag, _float fX, _float fY, _float fSizeX, _float fSizeY);
	
	// ��Ƽ����Ÿ���� �˻��ؼ� ��Ƽ����Ÿ�ٵ��� �����Ѵ�.
	HRESULT Render_Debug(const _tchar* pMRTTag, class CShader* pShader, class CVIBuffer_Rect* pVIBuffer);
#endif // DEBUG

private:
	// ����Ÿ�ٵ��� ��Ƶд�.
	unordered_map<const _tchar*, class CRenderTarget*> m_RenderTargets;

	// ��Ƽ����Ÿ��. (���ÿ� ��ġ�� ���ε��Ǿ���� ����Ÿ�ٵ��� ��Ƶд�.) */
	unordered_map<const _tchar*, list<class CRenderTarget*>> m_MRTs;

private:
	// ����� ����� ��� ����
	ID3D11RenderTargetView* m_pBackBufferView = { nullptr };
	ID3D11RenderTargetView* m_pPreBackBufferView = { nullptr };

	// ���̹��۴� 1���� ����ϴµ� �׳� ������ ��ġ�� ��� ���ε��Ѵ�.
	ID3D11DepthStencilView* m_pDepthStencilView = { nullptr };

private:
	class CRenderTarget* Find_RenderTarget(const _tchar* pTargetTag);
	list<class CRenderTarget*>* Find_MRT(const _tchar* pMRTTag);

public:
	virtual void Free(void) override;
};

END