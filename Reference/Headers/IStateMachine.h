#pragma once

#include "Engine_Defines.h"

BEGIN(Engine)
__interface ENGINE_DLL IStateMachine
{
public:
	virtual void OnStateEnter() = 0;
	virtual void OnStateTick(double TimeDelta) = 0;
	virtual void OnStateExit() = 0;
};
END