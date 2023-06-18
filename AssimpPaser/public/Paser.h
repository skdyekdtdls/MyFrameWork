#pragma once

#include "Engine_Defines.h"

class CPaser
{
public:
	CPaser() = default;
	virtual ~CPaser() = default;

public:
	HRESULT Pasing(const char* pModelFilePath);
	HRESULT Load(const _tchar* pModelFilePath, _Inout_ SCENE& tScene);
};