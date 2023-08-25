
#pragma once

#include "Client_Defines.h"
#include "GameObject.h"
#include "ISerializable.h"
BEGIN(Engine)

class CShader;
class CTexture;
class CRenderer;
class CTransform;
class CCollider;
class CVIBuffer_Sphere;
// Can declare VIBuffer or Model Com
END

BEGIN(Client)
class Dissolve;

class ForceField final : public CGameObject
{
public:
	typedef struct tagForceFieldDesc : public tagCGameObjectDesc
	{
		tagForceFieldDesc() : tagCGameObjectDesc() {}
		_float fRadius;
	};
private:
	ForceField(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	ForceField(const ForceField& rhs);
	virtual ~ForceField() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_double TimeDelta) override;
	virtual void Late_Tick(_double TimeDelta) override;
	virtual HRESULT Render() override;

	void Use();

private:
	void SetEnable(_bool bEnable) { m_bEnable = bEnable; }

private: /* For. Component */
	CShader* m_pShaderCom = { nullptr };
	CRenderer* m_pRendererCom = { nullptr };
	CTexture* m_pTextureCom = { nullptr };
	CTransform* m_pTransformCom = { nullptr };
	CVIBuffer_Sphere* m_pVIBufferCom = { nullptr };
	CColliderSphere* m_pColliderCom = { nullptr };
	Dissolve* m_pDissolve = { nullptr };

private:
	

private:
	_bool m_bEnable = { false };
	_bool m_bStart = { false };
	_bool m_bDelete = { false };
	_double m_dTimeAcc = { 0.0 };
	_double m_dDuration = { 5.0 };
	_float m_fRadius = { 1.f };
	_float m_fOffsetY = { 0.5f };
	CTransform* m_pOwnerTransform = { nullptr };
	_double m_TimeDelta;
	// Can declare VIBuffer or Model Com

private:
	HRESULT Add_Components();
	HRESULT SetUp_ShaderResources();

private:
	static _uint ForceField_Id;

public:
	static const _tchar* ProtoTag() { return L"Prototype_GameObject_ForceField"; }
	static ForceField* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free(void) override;
};
END

