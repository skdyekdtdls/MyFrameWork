#pragma once

#include "Assimp_Defines.h"
#include "Engine_Defines.h"

class CModelPaser
{
public:
	CModelPaser() = default;
	virtual ~CModelPaser() = default;

public:
	HRESULT Pasing(const char* pModelFilePath, ANIM_TYPE eAnimType);
	HRESULT Ready_Paser();
	void Relase_Paser();

private:
	Assimp::Importer	m_Importer; /* 내가 경로로 던져준 파일을 읽고 저장한다.*/
	const aiScene* m_pAIScene = { nullptr };

private:
	BONES							m_Bones;
	class CBonePaser*				m_pBonePaser = { nullptr };

private:
	_uint							m_iNumMeshes = { 0 };
	class CMeshPaser*				m_pMeshPaser = { nullptr };

private:
	_uint							m_iNumMaterials = { 0 };
	class CMaterialPaser*			m_pMaterialPaser = { nullptr };


private:
	_uint							m_iNumAnimations = { 0 };
	class CAnimationPaser*			m_pAnimationPaser = { nullptr };
};

