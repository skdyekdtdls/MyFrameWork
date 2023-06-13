#pragma once

#include "Assimp_Defines.h"

class CMaterialPaser
{
public:
	explicit CMaterialPaser() = default;
	virtual ~CMaterialPaser() = default;

public:
	void Pasing(HANDLE hFile, DWORD& dwByte, const aiScene* pAIScene, const char* pModelFilePath);
};

