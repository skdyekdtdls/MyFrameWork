#pragma once

#include "Assimp_Defines.h"

class CMeshPaser
{
public:
	CMeshPaser() = default;
	virtual ~CMeshPaser() = default;

	//HRESULT Pasing(HANDLE hFile, aiMesh* pAIMesh, DWORD& dwByte, ANIM_TYPE eAnimType, const BONES& BonesName);
	HRESULT Pasing(HANDLE hFile, DWORD& dwByte, const aiScene* pAIScene, ANIM_TYPE eAnimType, const BONES& BonesName);

private:
	void Pasing_SubMesh(HANDLE hFile, DWORD& dwByte, const aiMesh* pAIMesh, ANIM_TYPE eAnimType, const BONES& BonesName);
	void Pasing_Vertex_NonAnim(HANDLE hFile, const aiMesh* pAIMesh, DWORD& dwByte);
	void Pasing_Vertex_Anim(HANDLE hFile, const aiMesh* pAIMesh, DWORD& dwByte, const BONES& BonesName);
};