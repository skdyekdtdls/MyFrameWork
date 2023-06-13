#pragma once

#include "Assimp_Defines.h"

class CModelPaser
{
public:
	CModelPaser() = default;
	virtual ~CModelPaser() = default;

public:
	HRESULT Pasing(ANIM_TYPE eModelType, const char* pModelFilePath);
	HRESULT Ready_Paser();
	void Relase_Paser();

private:
	Assimp::Importer	m_Importer; /* ���� ��η� ������ ������ �а� �����Ѵ�.*/
	const aiScene* m_pAIScene = { nullptr };

private:
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

