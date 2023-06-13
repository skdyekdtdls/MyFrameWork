#pragma once

#include "Assimp_Defines.h"

class CMeshPaser
{
public:
	CMeshPaser() = default;
	virtual ~CMeshPaser() = default;

	HRESULT Pasing(aiMesh* pAIMesh, const char* pModelFilePath);


};

