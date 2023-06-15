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
private:
	Assimp::Importer	m_Importer; /* 내가 경로로 던져준 파일을 읽고 저장한다.*/
	const aiScene* m_pAIScene = { nullptr };
};

