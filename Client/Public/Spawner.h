
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
class Spawner final : public CGameObject
{
	typedef struct tagSpawnerDesc : public tagCGameObjectDesc
	{
		tagSpawnerDesc() : tagCGameObjectDesc() {}
	};
private:
	Spawner(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	Spawner(const Spawner& rhs);
	virtual ~Spawner() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_double TimeDelta) override;

private: /* For. Component */
	Observer* m_pObserver = { nullptr };
	CNavigation* m_pNavigationCom = { nullptr };
	// Can declare VIBuffer or Model Com

private:
	void Add_Event();
	HRESULT Add_Components();

	template <typename CLASS>
	void SpawnMonster(void* pArg);
	
private:
	static _uint Spawner_Id;

	_int m_iLeftNum;
	_int m_iRightNum;
	_uint m_ClintCellIndex;
public:
	static const _tchar* ProtoTag() { return L"Prototype_GameObject_Spawner"; }
	static Spawner* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free(void) override;
};
END

template<typename CLASS>
inline void Spawner::SpawnMonster(void* pArg)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);
	
	CGameObject* pObj = ObjectPool<CLASS>::GetInstance()->PopPool(CLASS::ProtoTag(), pArg);
	pGameInstance->AddToLayer(pGameInstance->Get_CurLevelIndex(), L"Layer_Monster", pObj);
	Safe_Release(pGameInstance);
}
