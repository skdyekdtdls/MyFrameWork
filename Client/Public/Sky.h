
#pragma once

#include "Client_Defines.h"
#include "GameObject.h"
#include "ISerializable.h"
BEGIN(Engine)

class CShader;
class CTexture;
class CRenderer;
class CTransform;
class CVIBuffer_Cube;

END

BEGIN(Client)
class Sky final : public CGameObject, public ISerializable
{
	typedef struct tagSkyDesc : public tagCGameObjectDesc
	{
		tagSkyDesc() : tagCGameObjectDesc() {}
	}SKY_DESC;
private:
	Sky(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	Sky(const Sky& rhs);
	virtual ~Sky() = default;

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
	CTexture* m_pTextureCom = { nullptr };
	CRenderer* m_pRendererCom = { nullptr };
	CTransform* m_pTransformCom = { nullptr };
	CVIBuffer_Cube* m_pVIBufferCom = { nullptr };

private:
	HRESULT Add_Components();
	HRESULT SetUp_ShaderResources();

private:
	static _uint Sky_Id;

public:
	static const _tchar* ProtoTag() { return L"Prototype_GameObject_Sky"; }
	static Sky* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free(void) override;
};
END

