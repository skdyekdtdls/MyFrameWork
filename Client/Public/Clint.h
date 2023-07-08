
#pragma once

#include "Client_Defines.h"
#include "GameObject.h"
#include "ISerializable.h"
#include "ClintModel.h"

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
class Pistola;
class ClintState;
END

BEGIN(Client)
class Clint final : public CGameObject, public ISerializable
{
public:
	typedef struct tagClintDesc : public tagCGameObjectDesc
	{
		tagClintDesc() : tagCGameObjectDesc() {}
	}CLINT_DESC;

private:
	Clint(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	Clint(const Clint& rhs);
	virtual ~Clint() = default;

public:
	void TransitionTo(const _tchar* pTag);

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_double TimeDelta) override;
	virtual void Late_Tick(_double TimeDelta) override;
	virtual HRESULT Render() override;

public:
	virtual void Save(HANDLE hFile, DWORD& dwByte) override;
	virtual void Load(HANDLE hFile, DWORD& dwByte, _uint iLevelIndex) override;

private: /* For. Component */
	ClintModel* m_pModelCom = { nullptr };
	CShader* m_pShaderCom = { nullptr };
	CTexture* m_pTextureCom = { nullptr };
	CRenderer* m_pRendererCom = { nullptr };
	CTransform* m_pTransformCom = { nullptr };
	CNavigation* m_pNavigationCom = { nullptr };
	CCollider* m_pColliderCom = { nullptr };
	Pistola* m_pPistolaComL = { nullptr };
	Pistola* m_pPistolaComR = { nullptr };

private:
	unordered_map<const _tchar*, ClintState*> m_pClintStates;
	ClintState*	m_pCurState = { nullptr };

private:
	static _uint Clint_Id;

private:
	void Add_State(const _tchar* pTag, ClintState* pState);
	ClintState* Find_State(const _tchar* pTag);
	HRESULT Add_Components();
	HRESULT SetUp_ShaderResources();

public:
	static const _tchar* ProtoTag() { return L"Prototype_GameObject_Clint"; }
	static Clint* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free(void) override;
};
END