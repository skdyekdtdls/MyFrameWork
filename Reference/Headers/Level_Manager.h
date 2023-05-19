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
	HRESULT Open_Level(CLevel* pNewLevel);

private:
	CLevel* m_pCurrentLevel = { nullptr };

public:
	void Free() override;
};

END