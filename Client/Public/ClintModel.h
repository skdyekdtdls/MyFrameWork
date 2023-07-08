#pragma once

#include "Client_Defines.h"
#include "Model.h"

BEGIN(Client)

class ClintModel final : public CModel
{
public:
	typedef struct tagClintModelDesc : public tagCModelDesc
	{
		tagClintModelDesc() : tagCModelDesc() {};
	}CLINT_MODEL_DESC;

private:
	explicit ClintModel(ID3D11Device * pDevice, ID3D11DeviceContext * pContext);
	explicit ClintModel(const ClintModel& rhs);
	virtual ~ClintModel() = default;

public: /* Getter*/
	//virtual class CBone* GetBoneByIndex(_uint iIndex) override;

public:
	void GroupingBones();
	virtual HRESULT Initialize(void* pArg) override;
	void Set_AnimByIndex(_uint iAnimIndex, BODY eBody);

public:
	virtual void ResetAnimation(_int iIndex = -1, BODY eBody = UPPER) override;
	virtual void Play_Animation(_double TimeDelta) override;
	virtual _bool IsAnimationFinished(BODY eBody) override;

private:
	_uint				m_iCurrentAnimIndex[BODY_END];
	_uint				m_iPrevAnimIndex[BODY_END];
	_double				m_InterTimeAcc[BODY_END];
	vector<class CAnimation*>	m_Animationss[BODY_END];

public:
	static ClintModel* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, _fmatrix PivotMatrix, _int RootIndex = -1);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END