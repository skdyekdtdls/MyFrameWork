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
	HRESULT Pasing(fs::path ModelFilePath);
	//HRESULT Load(const _tchar* pModelFilePath, _Inout_ SCENE& tScene);

	void MoveEntitySourToDest(fs::path SourPath, fs::path DestPath);
	CModel::TYPE IsAnim(fs::path ModelFilePath);
	void MakeDataFile(fs::path datFilePath);
	string RemoveSkeletonFromFilename(const string& filename);
	
private:
	Assimp::Importer	m_Importer;
	const aiScene* m_pAIScene = { nullptr };
	fs::path m_fsModelFilePath;
	CModel::TYPE m_eAnimType;
};