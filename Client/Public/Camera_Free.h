#pragma once

#include "Client_Defines.h"
#include "ISerializable.h"
#include "Camera.h"

BEGIN(Client)
class CCamera_Free final : public CCamera, public ISerializable
{
public:
	typedef struct tagFreeCameraDesc
	{
		_uint	iData = { 0 };
		CCamera::CAMERADESC	CameraDesc;
	}CAMERAFREEDESC;

private:
	CCamera_Free(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CCamera_Free(const CCamera_Free& rhs);
	virtual ~CCamera_Free() = default;

public:
	HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_double TimeDelta) override;
	virtual void Late_Tick(_double TimeDelta) override;
	virtual HRESULT Render() override;

	// ISerializable을(를) 통해 상속됨
	virtual void Save(HANDLE hFile, DWORD& dwByte) override;
	virtual void Load(HANDLE hFile, DWORD& dwByte, _uint iLevelIndex) override;

private:
	_uint	m_iData = { 0 };

public:
	static const _tchar* ProtoTag() { return L"Prototype_GameObject_Camera_Free"; }
	static CCamera_Free* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free(void) override;
};
END
