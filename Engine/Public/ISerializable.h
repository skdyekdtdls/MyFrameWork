#pragma once

#include "Engine_Defines.h"

BEGIN(Engine)
__interface ENGINE_DLL ISerializable
{
public:
	virtual void Save(HANDLE hFile, DWORD& dwByte) = 0;
	virtual void Load(HANDLE hFile, DWORD& dwByte, _uint iLevelIndex) = 0;
};
END