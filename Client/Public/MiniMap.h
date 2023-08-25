
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
END

BEGIN(Client)
class MiniMap final : public CGameObject
{
	typedef struct tagMiniMapDesc : public tagCGameObjectDesc
	{
		tagMiniMapDesc() : tagCGameObjectDesc() {}
	};
private:
	MiniMap(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	MiniMap(const MiniMap& rhs);
	virtual ~MiniMap() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_double TimeDelta) override;
	virtual void Late_Tick(_double TimeDelta) override;

private: /* For. Component */
	vector<Image*> m_pMonsterImages;
	list<Image*> m_pRenderImages;
	Image* m_pRaderImage = { nullptr };
	Image* m_pPlayerImage = { nullptr };
	Image* m_pArrowImage = { nullptr };
	Image* m_pBossImage = { nullptr };

private:
	_float4x4 m_WorldMatrixInverse;
	_float4x4 m_WorldMatrix;
	_float2	m_RaderPos;
	_float m_fRatio = { 0.1f };
private:
	HRESULT Add_Components();
	void CalaulateMatrices();
	void CalculateMonsterImagesPosition();

private:
	static _uint MiniMap_Id;

public:
	static const _tchar* ProtoTag() { return L"Prototype_GameObject_MiniMap"; }
	static MiniMap* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free(void) override;
};
END

