
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
class Dissolve final : public CGameObject
{
public:
	typedef struct tagDissolveDesc : public tagCGameObjectDesc
	{
		tagDissolveDesc() : tagCGameObjectDesc() {}
	}DISSOLVE_DESC;

private:
	Dissolve(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	Dissolve(const Dissolve& rhs);
	virtual ~Dissolve() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;

	HRESULT Bind_Values(CShader* pShader, _float fSumAmount, _float fEdgeThickness = 0.02f, _float4 vDissolveColor = { 1.f, 0.5f, 0.f, 1.f });
	_float DissoveProgress() { 1.f - m_fDissolveAmount; }
	_float GetDissolveAmount() { return m_fDissolveAmount; }
	void Reset();

	void SetDissolveAmount(_float fAmount) { m_fDissolveAmount = fAmount; }
private: /* For. Component */
	CTexture* m_pTextureCom = { nullptr }; // 노이즈텍스처

private:
	_float m_fDissolveAmount = { 1.f };
	_float m_fEdgeThickness = { 0.02f }; // 그을림 두께를 결정
	_float4 m_vDissolveColor = { 1.f, 0.5f, 0.f, 1.f };
	// Can declare VIBuffer or Model Com

private:
	HRESULT Add_Components();

private:
	static _uint Dissolve_Id;

public:
	static const _tchar* ProtoTag() { return L"Prototype_GameObject_Dissolve"; }
	static Dissolve* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free(void) override;
};
END

