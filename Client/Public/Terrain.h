#pragma once

#include "Client_Defines.h"
#include "GameObject.h"
#include "ISerializable.h"

BEGIN(Engine)

class CShader;
class CTexture;
class CRenderer;
class CTransform;
class CVIBuffer_Terrain;
class CNavigation;
END

BEGIN(Client)
class CTerrain final : public CGameObject, public ISerializable
{
public:
	enum TEXTURETYPE { TYPE_DIFFUSE, TYPE_MASK, TYPE_BRUSH, TYPE_END };

private:
	CTerrain(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CTerrain(const CTerrain& rhs);
	CTerrain& operator=(const CTerrain& rhs) = delete;
	virtual ~CTerrain() = default;

public:
	// ISerializable을(를) 통해 상속됨
	virtual void Save(HANDLE hFile, DWORD& dwByte) override;
	virtual void Load(HANDLE hFile, DWORD& dwByte, _uint iLevelIndex) override;
public:
	_uint GetCellNum();

public:
	HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_double TimeDelta) override;
	virtual void Late_Tick(_double TimeDelta) override;
	virtual HRESULT Render() override;
	void AddCell(const _float3* vPoints);
	
#ifdef _DEBUG
public:
	virtual _bool Picked(_Inout_ PICK_DESC& tPickDesc, const RAY& tRay) override;
#endif DEBUG

private:
	CShader* m_pShaderCom = { nullptr };
	CTexture* m_pTextureCom[TYPE_END] = { nullptr };
	CRenderer* m_pRendererCom = { nullptr };
	CTransform* m_pTransformCom = { nullptr };
	CVIBuffer_Terrain* m_pVIBufferCom = { nullptr };
	CNavigation* m_pNavigationCom = { nullptr };

private:
	HRESULT Add_Components();
	HRESULT SetUp_ShaderResources();

public:
	static _uint CTerrain_Id;
	static const _tchar* ProtoTag() { return L"Prototype_GameObject_CTerrain"; }
	static CTerrain* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free(void) override;
};
END