#pragma once

#include "Assimp_Defines.h"
#include "Engine_Defines.h"

class Paser
{
public:
	Paser() = default;
	virtual ~Paser() = default;

public:
	HRESULT Pasing(const char* pModelFilePath, ANIM_TYPE eAnimType);
	HRESULT Load(const char* pModelFilePath);
};

