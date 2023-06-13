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
#include <d3d11.h>

#include "Struct.h"
#include "Function.h"

namespace fs = std::filesystem;
using namespace std;

#ifdef _DEBUG

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#ifndef DBG_NEW 

#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ ) 
#define new DBG_NEW 

#endif
#endif

typedef vector<const char*> BONES;