#pragma once

#include "GameObject.h"
#include "Transform.h"
#include "ISerializable.h"
BEGIN(Engine)
class ENGINE_DLL CCamera abstract : public CGameObject, public ISerializable
{
public:
	typedef struct tagCameraDesc : public tagCGameObjectDesc
	{
		tagCameraDesc() : tagCGameObjectDesc() {};

		_float4 vEye, vAt, vUp;
		_float	fFovy, fAspect, fNear, fFar;
		CTransform::CTRANSFORM_DESC TransformDesc;
	}CAMERADESC;

protected:
	CCamera(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CCamera(const CCamera& rhs);
	CCamera operator=(const CCamera& rhs) = delete;
	virtual ~CCamera() = default;

public:
	virtual void Save(HANDLE hFile, DWORD& dwByte) override;
	virtual void Load(HANDLE hFile, DWORD& dwByte, _uint iLevelIndex) override;
public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_double TimeDelta) override;
	virtual void Late_Tick(_double TimeDelta) override;
	virtual HRESULT Render() override;

protected:
	_float4				m_vEye, m_vAt, m_vUp;
	_float				m_fFovy, m_fAspect, m_fNear, m_fFar;

	class CTransform*	m_pTransform = { nullptr };
	class CPipeLine*	m_pPipeLine = { nullptr };

public:
	virtual CGameObject* Clone(void* pArg) = 0;
	virtual void Free() override;
};

END