#pragma once

#include "Engine_Defines.h"
#include <filesystem>
namespace fs = std::filesystem;

class CPaser
{
public:
	CPaser() = default;
	virtual ~CPaser() = default;

public:
	HRESULT Pasing(fs::path ModelFilePath);
	//HRESULT Load(const _tchar* pModelFilePath, _Inout_ SCENE& tScene);
};