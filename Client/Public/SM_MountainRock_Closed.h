
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
class SM_MountainRock_Closed final : public CGameObject, public ISerializable
{
	typedef struct tagSM_MountainRock_ClosedDesc : public tagCGameObjectDesc
	{
		tagSM_MountainRock_ClosedDesc() : tagCGameObjectDesc() {}
	};
private:
	SM_MountainRock_Closed(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	SM_MountainRock_Closed(const SM_MountainRock_Closed& rhs);
	virtual ~SM_MountainRock_Closed() = default;

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
	static _uint SM_MountainRock_Closed_Id;

public:
	static const _tchar* ProtoTag() { return L"Prototype_GameObject_SM_MountainRock_Closed"; }
	static SM_MountainRock_Closed* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free(void) override;
};
END
