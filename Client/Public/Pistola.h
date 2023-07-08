
#pragma once

#include "Client_Defines.h"
#include "GameObject.h"
#include "ISerializable.h"
BEGIN(Engine)

class CShader;
class CRenderer;
class CTransform;
class CModel;
END

BEGIN(Client)
class Pistola final : public CGameObject, public ISerializable
{
public:
	
	typedef struct tagPistolaDesc : public tagCGameObjectDesc
	{
		tagPistolaDesc() : tagCGameObjectDesc() {}
		const char* pAttachedBoneName;

	}PISTOLA_DESC;

private:
	Pistola(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	Pistola(const Pistola& rhs);
	virtual ~Pistola() = default;

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

private: /* ºÎ¸ð²¨ */
	_float4x4	m_OffsetMatrix;
	_float4x4	m_PivotMatrix;
	const _float4x4* m_pCombindTransformationMatrix;
	const _float4x4* m_pParentWorldMatrix;
	_float4x4	m_WorldMatrix;
	

	_bool m_bStart = { true };
private:
	HRESULT Add_Components();
	HRESULT SetUp_ShaderResources();

private:
	static _uint Pistola_Id;

public:
	static const _tchar* ProtoTag() { return L"Prototype_GameObject_Pistola"; }
	static Pistola* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free(void) override;
};
END