#pragma once
#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CNavigation :
	public CComponent
{
public:
	typedef struct tagNavigation
	{
		tagNavigation() : iCurrentIndex(-1) {}
		explicit tagNavigation(_int _iCurrentIndex) : iCurrentIndex{ _iCurrentIndex } {}

		_int iCurrentIndex = { -1 };
	}NAVIGATIONDESC;

private:
	explicit CNavigation(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit CNavigation(const CNavigation& rhs);
	virtual ~CNavigation() = default;

public:
	virtual HRESULT Initialize_Prototype(const _tchar* pNavigationDataFiles);
	virtual HRESULT Initialize(void* pArg) override;

#ifdef _DEBUG
	void Set_ShaderResources();
	HRESULT Render_Navigation();
#endif

private:
	NAVIGATIONDESC	m_tNaviDesc;
	vector<class CCell*>	m_Cells;

#ifdef _DEBUG
private:
	class CShader* m_pShader = { nullptr };
#endif // _DEBUG


public:
	static const _tchar* ProtoTag() { return L"Prototype_Component_CNavigation"; }
	static CNavigation* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _tchar* pNavigationDataFiles);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END