#pragma once

#include "Engine_Defines.h"

BEGIN(Engine)
class IObserver
{
public:
	virtual void Subscribe(const _tchar* pTag, function<void()> CallBack) = 0;
	virtual void UnSubscribe(const _tchar* pTag) = 0;
	virtual void UnSubscribeDelay(const _tchar* pTag) = 0;
};

END