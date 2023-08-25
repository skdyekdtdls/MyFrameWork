
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
class CNavigation;
class CModel;
// Can declare VIBuffer or Model Com
END

BEGIN(Client)
class BushA final : public CGameObject, public ISerializable
{
	typedef struct tagBushADesc : public tagCGameObjectDesc
	{
		tagBushADesc() : tagCGameObjectDesc() {}
	};
private:
	BushA(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	BushA(const BushA& rhs);
	virtual ~BushA() = default;

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
	CShader* m_pShaderCom = { nullptr };
	CRenderer* m_pRendererCom = { nullptr };
	CTransform* m_pTransformCom = { nullptr };
	CModel* m_pModelCom = { nullptr };
	// Can declare VIBuffer or Model Com
private:
	HRESULT Add_Components();
	HRESULT SetUp_ShaderResources();

private:
	static _uint BushA_Id;

public:
	static const _tchar* ProtoTag() { return L"Prototype_GameObject_BushA"; }
	static BushA* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free(void) override;
};
END

