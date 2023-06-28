#pragma once

#include "Engine_Defines.h"
#include <filesystem>
#include "GameInstance.h"
#include "Model.h"



namespace fs = std::filesystem;

class CPaser
{
public:
	CPaser() = default;
	virtual ~CPaser() = default;

public:
	HRESULT Pasing(fs::path ModelFilePath, CModel::TYPE& Out);
	HRESULT WriteAnimData();
	HRESULT Ready_Bones(const aiNode* pNode, CBone* pParent);
	//HRESULT Load(const _tchar* pModelFilePath, _Inout_ SCENE& tScene);
};