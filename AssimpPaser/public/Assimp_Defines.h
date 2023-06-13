#pragma once
#include "assimp\Importer.hpp"
#include "assimp\scene.h"
#include "assimp\postprocess.h"
#include <string>
#include <filesystem>
#include <iostream>
#include <Windows.h>
#include <tchar.h>
#include <algorithm>
#include <regex>

#include "Struct.h"
#include "Function.h"

#define FAILED_CHECK_RETURN(_hr, _return) \
	{if ((HRESULT)_hr < 0) { __debugbreak(); return _return; }}

#define ZeroStruct(Desc) ZeroMemory((&Desc), (sizeof(Desc)));

namespace fs = std::filesystem;
using namespace std;

typedef UINT _uint;

#define MAX_PATH 260

#ifdef _DEBUG

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#ifndef DBG_NEW 

#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ ) 
#define new DBG_NEW 

#endif
#endif