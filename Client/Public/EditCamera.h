#pragma once

#include "Client_Defines.h"
#include "Camera.h"

BEGIN(Client)
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

private:
	_uint	m_iData = { 0 };

public:
	static const _tchar* ProtoTag() { return L"Prototype_GameObject_Camera_Edit"; }
	static CEditCamera* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free(void) override;
};
END
