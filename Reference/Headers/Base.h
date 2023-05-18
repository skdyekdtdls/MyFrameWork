#pragma once

#include "Engine_Defines.h"

BEGIN(Engine)

class ENGINE_DLL CBase abstract
{
protected:
	CBase() = default;
	virtual ~CBase() = default;

public:
	_ulong Add_Ref();
	_ulong Release();

private:
	_ulong m_dwRefCnt = { 0 };
public:
	virtual void Free() = 0;
};

END