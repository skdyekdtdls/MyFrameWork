#pragma once

#include "Assimp_Defines.h"

class CModelPaser
{
public:
	CModelPaser() = default;
	virtual ~CModelPaser() = default;

public:
	HRESULT Pasing(TYPE eModelType, const char* pModelFilePath);

private:
	Assimp::Importer	m_Importer; /* 내가 경로로 던져준 파일을 읽고 저장한다.*/
	const aiScene* m_pAIScene = { nullptr };

private:
	TYPE							m_eModelType = { TYPE_END };

private:
	_uint							m_iNumMeshes = { 0 };
	class CMeshPaser*				m_MeshPaser = { nullptr };

private:
	_uint							m_iNumMaterials = { 0 };
	class CMaterialPaser*			m_MaterialPaser = { nullptr };

private:
	class CBonePaser*				m_BonePaser = { nullptr };

private:
	_uint							m_iNumAnimations = { 0 };
	class CAnimationPaser*			m_AnimationPaser = { nullptr };
};

