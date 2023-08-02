
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
class SkillQUI final : public CGameObject
{
	typedef struct tagSkillQUIDesc : public tagCGameObjectDesc
	{
		tagSkillQUIDesc() : tagCGameObjectDesc() {}
	};
private:
	SkillQUI(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	SkillQUI(const SkillQUI& rhs);
	virtual ~SkillQUI() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_double TimeDelta) override;
	virtual void Late_Tick(_double TimeDelta) override;
	virtual HRESULT Render() override;

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
	static _uint SkillQUI_Id;

public:
	static const _tchar* ProtoTag() { return L"Prototype_GameObject_SkillQUI"; }
	static SkillQUI* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free(void) override;
};
END

