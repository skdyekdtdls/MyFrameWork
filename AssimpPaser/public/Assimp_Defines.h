#pragma once
#include "assimp\Importer.hpp"
#include "assimp\scene.h"
#include "assimp\postprocess.h"
#include <string>
#include <filesystem>
#include <iostream>
#include <Windows.h>
#include <tchar.h>

namespace fs = std::filesystem;
using namespace std;

typedef UINT _uint;

#define MAX_PATH 260

enum TYPE { TYPE_NONANIM, TYPE_ANIM, TYPE_END };

#include "Struct.h"
#include "Function.h"