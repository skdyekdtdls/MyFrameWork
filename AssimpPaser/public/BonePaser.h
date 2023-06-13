#pragma once

#include "Assimp_Defines.h"

class CBonePaser
{
public:
	CBonePaser() = default;
	virtual ~CBonePaser() = default;

public:
	HRESULT Pasing(HANDLE hFile, DWORD& dwByte, aiNode* pNode, class CBone* pParent);
};

