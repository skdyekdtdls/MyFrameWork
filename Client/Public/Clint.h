
#pragma once

#include "Client_Defines.h"
#include "GameObject.h"
#include "ISerializable.h"
BEGIN(Engine)

class CShader;
class CTexture;
class CRenderer;
class CTransform;
class CModel;
class CCollider;
class CNavigation;
END

BEGIN(Client)
class Clint final : public CGameObject, public ISerializable
{
private:
	Clint(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	Clint(const Clint& rhs);
	virtual ~Clint() = default;

public:
	void Set_AnimState(CLINT_ANIM eAnimState);

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_double TimeDelta) override;
	virtual void Late_Tick(_double TimeDelta) override;
	virtual HRESULT Render() override;

public:
	virtual void Save(HANDLE hFile, DWORD& dwByte) override;
	virtual void Load(HANDLE hFile, DWORD& dwByte, _uint iLevelIndex) override;

private:
	void KeyInput(_double& TimeDelta);

private: /* For. Component */
	CModel* m_pModelCom = { nullptr };
	CShader* m_pShaderCom = { nullptr };
	CTexture* m_pTextureCom = { nullptr };
	CRenderer* m_pRendererCom = { nullptr };
	CTransform* m_pTransformCom = { nullptr };
	CNavigation* m_pNavigationCom = { nullptr };
	CCollider* m_pColliderCom = { nullptr };
	// Can declare VIBuffer or Model Com

private:
	CLINT_ANIM m_eCurState = { CLINT_ANIM::IDLE };
	CLINT_ANIM m_ePreState;

private:
	static _uint Clint_Id;

private:
	void Shoot_FSM(_double TimeDelta);
	void Run_FSM(_double TimeDelta);
	void Idle_FSM(_double TimeDelta);
	void Dash_FSM(_double TimeDelta);

private:
	HRESULT Add_Components();
	HRESULT SetUp_ShaderResources();

public:
	static const _tchar* ProtoTag() { return L"Prototype_GameObject_Clint"; }
	static Clint* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free(void) override;
};
END

