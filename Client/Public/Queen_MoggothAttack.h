
#pragma once
#include "Client_Defines.h"
#include "StateMachine.h"

BEGIN(Client)
class Queen_Moggoth;
class Queen_MoggothAttack final : public StateMachine<Queen_Moggoth, QUEEN_MOGGOTH_ANIM>
{
	enum PATTERN { ATTACK01, ATTACK02, ATTACK03, ATTACK04, PATTERN_END };
private:
	Queen_MoggothAttack(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	Queen_MoggothAttack(const Queen_MoggothAttack& rhs);
	virtual ~Queen_MoggothAttack() = default;

public:
	virtual void OnStateEnter() override;
	virtual void OnStateTick(_double TimeDelta) override;
	virtual void OnStateExit() override;

	virtual void OnCollision(CCollider::COLLISION_INFO tCollisionInfo, _double TimeDelta) override;

	virtual const _tchar* GetTag() override { return Tag(); }

private:
	_double m_TimeAcc = { 0.0 };
	_uint m_iLogic = { 0 };
	PATTERN m_ePattern = { PATTERN_END };
	PATTERN m_ePrevPattern = { PATTERN_END };
	CModel* m_pModel = { nullptr };
	CTransform* m_pTransform = { nullptr };
	CNavigation* m_pNavigation = { nullptr };
	CCollider* m_pAttack04Coll = { nullptr };

private:
	void Attack01(const _double& TimeDelta); // ÀÔ¿¡¼­ ¹¹ ¹ß»ç
	void Attack02(const _double& TimeDelta); // ²¿¸® ¶¥¿¡ ¹Ú°í ²¿¸®·ÎºÎÅÍ ºÎÃ¤²Ã·Î °ø°Ý
	void Attack03(const _double& TimeDelta); // ¶¥¿¡ ²¿¸® ¹Ú°í ¹Ø¿¡¼­ ¹¹ ¿Ã¶ó¿È.
	void Attack04(const _double& TimeDelta); // zÃàÈ¸Àü°ø°Ý
	void SetAnimIndexByPattern();

public:
	static const _tchar* Tag() { return L"Queen_MoggothAttack"; }
	static Queen_MoggothAttack* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual StateMachine<Queen_Moggoth, QUEEN_MOGGOTH_ANIM>* Clone(void* pArg) override;
	virtual void Free() override;
};

END