#pragma once

#include "Assimp_Defines.h"

class CMeshPaser
{
public:
	CMeshPaser() = default;
	virtual ~CMeshPaser() = default;

	HRESULT Pasing(HANDLE hFile, aiMesh* pAIMesh, DWORD& dwByte, ANIM_TYPE eAnimType, const BONES& BonesName);

private:
	virtual void Pasing_Vertex_NonAnim(HANDLE hFile, aiMesh* pAIMesh, DWORD& dwByte);
	virtual void Pasing_Vertex_Anim(HANDLE hFile, aiMesh* pAIMesh, DWORD& dwByte, const BONES& BonesName);
};