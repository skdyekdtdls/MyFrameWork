#pragma once

#include "Base.h"

BEGIN(Engine)
class CLevel;
class CLevel_Manager final : public CBase
{
	DECLARE_SINGLETON(CLevel_Manager)
public:
	CLevel_Manager();
	~CLevel_Manager() = default;

public:
	const _uint& Get_CurLevelIndex() {
		return m_iLevelindex;
	}

	const _uint& Get_NextLevelIndex() {
		return m_iNextLevelindex;
	}
	void Set_NextLevelIndex(_uint iIndex) {
		m_iNextLevelindex = iIndex;
	}

	HRESULT Open_Level(_uint iLevelindex, CLevel* pNewLevel);
	void Tick(_double TimeDelta);
	void Late_Tick(_double TimeDelta);

private:
	CLevel* m_pCurrentLevel = { nullptr };
	_uint	m_iLevelindex = { 0 };
	_uint	m_iNextLevelindex = { 0 };

public:
	void Free() override;
};

END