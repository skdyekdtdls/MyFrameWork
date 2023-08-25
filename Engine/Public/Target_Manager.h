#pragma once

#include "Base.h"

// 렌더타겟을 여러개 생성하여 저장하기 위해서

BEGIN(Engine)

class CTarget_Manager final : public CBase
{
	DECLARE_SINGLETON(CTarget_Manager)
private:
	CTarget_Manager() = default;
	virtual ~CTarget_Manager() = default;

public:
	// 렌더타겟 생성 후 추가
	HRESULT Add_RenderTarget(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _tchar* pTargetTag, _uint iSizeX, _uint iSizeY, DXGI_FORMAT eFormat, const _float4& vClearColor);;
	
	// 렌더타겟을 검색하여 멀티렌더타겟 그룹에 넣는다.(MRT가 없을시 새로 생성함.)
	HRESULT Add_MRT(const _tchar* pMRTTag, const _tchar* pTargetTag);

	/* pMRTTag에 추가되어있는 렌더타겟들을 장치에 바인딩한다. */
	HRESULT Begin_MRT(ID3D11DeviceContext* pContext, const _tchar* pMRTTag);
	HRESULT End_MRT(ID3D11DeviceContext* pContext);

	/* 원래상태로 복구한다. (0번째에 백버퍼가 바인딩 된 상태로 돌려준다. */
	HRESULT Begin_PreMRT(ID3D11DeviceContext* pContext, const _tchar* pMRTTag);
	HRESULT End_PreMRT(ID3D11DeviceContext* pContext);

	// 렌더타겟을 검색 후 인자로 들어온 셰이더의 변수에 바인딩한다.
	HRESULT Bind_ShaderResourceView(const _tchar* pTargetTag, class CShader* pShader, const char* pConstantName);

#ifdef _DEBUG
public:
	// 렌더타겟을 검색해서 Ready_Debug함수를 호출한다.
	HRESULT Ready_Debug(const _tchar* pTargetTag, _float fX, _float fY, _float fSizeX, _float fSizeY);
	
	// 멀티렌더타겟을 검색해서 멀티렌더타겟들을 렌더한다.
	HRESULT Render_Debug(const _tchar* pMRTTag, class CShader* pShader, class CVIBuffer_Rect* pVIBuffer);
#endif // DEBUG

private:
	// 렌더타겟들을 모아둔다.
	unordered_map<const _tchar*, class CRenderTarget*> m_RenderTargets;

	// 멀티렌더타겟. (동시에 장치에 바인딩되어야할 렌더타겟들을 모아둔다.) */
	unordered_map<const _tchar*, list<class CRenderTarget*>> m_MRTs;

private:
	// 백업용 백버퍼 멤버 변수
	ID3D11RenderTargetView* m_pBackBufferView = { nullptr };
	ID3D11RenderTargetView* m_pPreBackBufferView = { nullptr };

	// 깊이버퍼는 1개만 사용하는데 그냥 같은거 장치에 계속 바인딩한다.
	ID3D11DepthStencilView* m_pDepthStencilView = { nullptr };

private:
	class CRenderTarget* Find_RenderTarget(const _tchar* pTargetTag);
	list<class CRenderTarget*>* Find_MRT(const _tchar* pMRTTag);

public:
	virtual void Free(void) override;
};

END