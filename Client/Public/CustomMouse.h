
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
class Image;

class CustomMouse final : public CGameObject
{
	typedef struct tagCustumMouseDesc : public tagCGameObjectDesc
	{
		tagCustumMouseDesc() : tagCGameObjectDesc() {}
	};
private:
	CustomMouse(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CustomMouse(const CustomMouse& rhs);
	virtual ~CustomMouse() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_double TimeDelta) override;
	virtual void Late_Tick(_double TimeDelta) override;
	virtual HRESULT Render() override;

private: /* For. Component */
	
	Image* m_pImageCom = { nullptr };
	//CShader* m_pShaderCom = { nullptr };
	//CRenderer* m_pRendererCom = { nullptr };
	//CTexture* m_pTexutreCom = { nullptr };
	//CVIBuffer_Rect* m_pVIBufferCom = { nullptr };
	//CTransform2D* m_pTransformCom = { nullptr };

private:
	_float4x4				m_ViewMatrix, m_ProjMatrix;
	// Can declare VIBuffer or Model Com
private:
	HRESULT Add_Components();
	HRESULT SetUp_ShaderResources();

private:
	static _uint CustomMouse_Id;

public:
	static const _tchar* ProtoTag() { return L"Prototype_GameObject_CustumMouse"; }
	static CustomMouse* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free(void) override;
};
END

