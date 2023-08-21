
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
	enum MODE { EDIT_MODE, PLAY_MODE, MODE_END };
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
	const RAY& GetMouseRay() {
		return m_tMouseRay;
	}
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

	void EditMode_Tick(_double TimeDelta);
	void PlayMode_Tick(_double TimeDelta);
	void EditMode_Late_Tick(_double TimeDelta);
	void PlayMode_Late_Tick(_double TimeDelta);

private:
	void Mouse_Input(_double TimeDelta);
	void Key_Input(_double TimeDelta);
	void Picking();

private:
	MODE		m_eEditMode = { PLAY_MODE };
	CRenderer* m_pRenderer = { nullptr };
	list<PICK_DESC> m_tPickDescs;
	RAY		m_tMouseRay;
	_bool   m_bStart = { true };
	_bool	m_isPicking = { false };
	_uint   m_iAttachingBoneIndex = { 0 };
	_float4 m_OffsetPos = { _float4(0.f, 10.f, -10.f, 0.f) };

private:
	void Make_MouseRay();

public:
	static const _tchar* ProtoTag() { return L"Prototype_GameObject_Camera_Edit"; }
	static CEditCamera* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free(void) override;

	friend class CImWindow_ObjectTool;
};
END

