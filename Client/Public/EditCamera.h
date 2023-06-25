#ifdef _DEBUG

#pragma once
#include "Client_Defines.h"
#include "Camera.h"
#include "Cell.h"

BEGIN(Engine)
class CRenderer;
class CGameInstance;
END

BEGIN(Client)
class CTerrain;

class CEditCamera final : public CCamera
{
public:
	typedef struct tagEditCameraDesc : public CAMERADESC
	{
		
	}EDIT_CAMERA_DESC;
private:
	CEditCamera(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CEditCamera(const CEditCamera& rhs);
	virtual ~CEditCamera() = default;

public:
	PICK_DESC GetMinDistPickDesc();
	PICK_DESC GetTerrainPickDesc();
	void AddPickDesc(PICK_DESC tPickDesc);
	void DeletePickDescByPtr(CGameObject* pGameObject);
	void ClearPickDesc();

	// LateTick에서 사용할 것을 권함. 카메라의 픽 정보는 Tick에서 업데이트됨.
	const _bool& IsPicking() {
		return m_isPicking;
	}
public:
	HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_double TimeDelta) override;
	virtual void Late_Tick(_double TimeDelta) override;
	virtual HRESULT Render() override;

private:
	void Mouse_Input(_double TimeDelta);
	void Key_Input(_double TimeDelta);
	void Picking();

private:
	CRenderer* m_pRenderer = { nullptr };
	list<PICK_DESC> m_tPickDescs;
	RAY		m_tMouseRay;
	_bool	m_isPicking = { false };

private:
	void Make_MouseRay();

public:
	static const _tchar* ProtoTag() { return L"Prototype_GameObject_Camera_Edit"; }
	static CEditCamera* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free(void) override;
};
END

#endif