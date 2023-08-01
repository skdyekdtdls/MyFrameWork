#pragma once

#include "Component.h"

BEGIN(Engine)
class CGameObject;
class ENGINE_DLL CRenderer final : public CComponent
{
public:
	typedef struct tagCRendererDesc : public tagComponentDesc
	{
		tagCRendererDesc() : tagComponentDesc() {};
	}CRENDERER_DESC;

public:
	enum RENDERGROUP { RENDER_PRIORITY, RENDER_NONBLEND, RENDER_NONLIGHT, RENDER_BLEND, RENDER_UI, RENDER_END };
private:
	CRenderer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CRenderer() = default;

public:
	const list<CGameObject*>& Get_RenderObjects(_uint iIndex) {
		return m_RenderObjects[iIndex];
	}

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	
	void Add_RenderGroup(RENDERGROUP eRenderGroup, CGameObject* pGameObejct);
	HRESULT Draw_RenderGroup();

#ifdef _DEBUG
	HRESULT Add_DebugGroup(CComponent* pDebugCom);
	HRESULT Render_Debug();
#endif // _DEBUG

private:
	HRESULT Render_Priority();
	HRESULT Render_NonBlend();
	HRESULT Render_Lights();
	HRESULT Render_Deferred();
	HRESULT Render_NonLight();
	HRESULT Render_Blend();
	HRESULT Render_UI();

private:
	list<CGameObject*> m_RenderObjects[RENDER_END];

private:
	list<class CComponent*>	m_DebugObject;

private:
	class CTarget_Manager* m_pTarget_Manager = { nullptr };
	class CLight_Manager* m_pLight_Manager = { nullptr };

private:
	class CVIBuffer_Rect*	m_pVIBuffer = { nullptr };
	class CShader*			m_pShader = { nullptr };
	_float4x4				m_WorldMatrix, m_ViewMatrix, m_ProjMatrix;

public:
	static const _tchar* ProtoTag() { return L"Prototype_Component_Renderer"; }
	static CRenderer* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free(void) override;
};

END