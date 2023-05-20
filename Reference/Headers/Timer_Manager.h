#pragma once

#include "Base.h"
BEGIN(Engine)
class CTimer;
class ENGINE_DLL CTimer_Manager final : public CBase
{
	DECLARE_SINGLETON(CTimer_Manager)
public:
	CTimer_Manager();
	~CTimer_Manager() = default;
	
public:
	HRESULT	 Ready_Timer(const _tchar * pTimerTag);
	_double	 Get_Timer(const _tchar* pTimerTag);
	void	 Set_Timer(const _tchar* pTimerTag);
private:
	unordered_map<const _tchar*, CTimer*>	m_pTimers;
private:
	CTimer* Find_Timer(const _tchar * pTimerTag);

public:
	virtual void Free(void) override;
};

END