
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
class RimLight final : public CGameObject
{
	typedef struct tagRimLightDesc : public tagCGameObjectDesc
	{
		tagRimLightDesc() : tagCGameObjectDesc() {}
	};
private:
	RimLight(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	RimLight(const RimLight& rhs);
	virtual ~RimLight() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	HRESULT Bind_Values(CShader* pShader, _float fRimPower, _float4 vRimColor = { 1.f, 0.f, 0.f, 1.f });

private:
	static _uint RimLight_Id;

public:
	static const _tchar* ProtoTag() { return L"Prototype_GameObject_RimLight"; }
	static RimLight* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free(void) override;
};
END

