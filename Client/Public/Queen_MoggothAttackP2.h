
#pragma once
#include "Client_Defines.h"
#include "StateMachine.h"

BEGIN(Client)
class Queen_Moggoth;
class Queen_MoggothAttackP2 final : public StateMachine<Queen_Moggoth, QUEEN_MOGGOTH_ANIM>
{
	enum PATTERN { ATTACK01, ATTACK02, ATTACK03, ATTACK04, PATTERN_END };
private:
	Queen_MoggothAttackP2(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	Queen_MoggothAttackP2(const Queen_MoggothAttackP2& rhs);
	virtual ~Queen_MoggothAttackP2() = default;

public:
	virtual void OnStateEnter() override;
	virtual void OnStateTick(_double TimeDelta) override;
	virtual void OnStateExit() override;

	virtual void OnCollision(CCollider::COLLISION_INFO tCollisionInfo, _double TimeDelta) override;

	virtual const _tchar* GetTag() override { return Tag(); }

private:
	_double m_TimeAcc = { 0.0 };
	_double m_TimeAccShot = { 0.0 };
	_uint m_iLogic = { 0 };
	PATTERN m_ePattern = { PATTERN_END };
	PATTERN m_ePrevPattern = { PATTERN_END };
	CModel* m_pModel = { nullptr };
	CTransform* m_pTransform = { nullptr };
	CNavigation* m_pNavigation = { nullptr };

	
private:
	void Attack01(const _double& TimeDelta); // ²¿¸®Ç«Âï
	void Attack02(const _double& TimeDelta); // ´ú´ú°Å¸®¸é¼­ ¶¥¿¡ ¹¹¿Ã¶ó¿È
	void Attack03(const _double& TimeDelta); // µÚ¿¡ ´Ù ²¿¸®ÈÖµÎ¸£±â
	void Attack04(const _double& TimeDelta); // ´ú´ú°Å¸®¸é¼­ ²¿¸®Æ÷Åº¹ß»ç.
	void SetAnimIndexByPattern();

public:
	static const _tchar* Tag() { return L"Queen_MoggothAttackP2"; }
	static Queen_MoggothAttackP2* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual StateMachine<Queen_Moggoth, QUEEN_MOGGOTH_ANIM>* Clone(void* pArg) override;
	virtual void Free() override;
};

END