#define _USE_IMGUI
#pragma once

#include "Client_Defines.h"
#include "Camera.h"
#include "Cell.h"

BEGIN(Engine)
class CRenderer;
END

BEGIN(Client)
class CTerrain;

class CEditCamera final : public CCamera
{
public:
	typedef struct tagEditCameraDesc
	{
		_uint	iData = { 0 };
		CCamera::CAMERADESC	CameraDesc;
	}EDIT_CAMERA_DESC;

private:
	CEditCamera(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CEditCamera(const CEditCamera& rhs);
	virtual ~CEditCamera() = default;

public:
	HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_double TimeDelta) override;
	virtual void Late_Tick(_double TimeDelta) override;
	virtual HRESULT Render() override;

private:
	void Mouse_Input(_double TimeDelta);
	void Key_Input(_double TimeDelta);

	void Late_Mouse_Input(_double TimeDelta);
private:
	_uint	m_iData = { 0 };
	RAY		m_tMouseRay;

private:
	CTerrain* m_pTerrain = { nullptr };

private: /* Components */
	CRenderer* m_pRendererCom = { nullptr };

private:
	HRESULT Add_Components();

private: /* For. Edit_Mode */
	void Object_Place(CGameInstance* pGameInstance);
	void Edit_Navigation_Mesh(CGameInstance* pGameInstance);
	void Edit_Transform(CGameInstance* pGameInstance);

private: /* For. Navigation Mdoe */
	_uint m_iClickCount = { 0 };
	_float3 m_vClickPoint[CCell::POINT_END];

	void Set_Terrain(class CTerrain* pTerrain);

	void CreateTriangleStrip(CGameInstance* pGameInstance);
	//void SelectPoint(CGameInstance* pGameInstance);
	//void SelectCellCGameInstance* pGameInstance();

	//void Render_Cells();

private:
	void Make_MouseRay();

public:
	static const _tchar* ProtoTag() { return L"Prototype_GameObject_Camera_Edit"; }
	static CEditCamera* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free(void) override;
};
END
