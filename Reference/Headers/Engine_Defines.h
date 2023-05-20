#pragma once

#pragma warning (disable : 4005)

#include <windows.h>
#include <iostream>
#include <tchar.h>
#include <d3d11.h>
#include <DirectXMath.h>
#include <d3dcompiler.h>
#include <cassert>
using namespace std;
using namespace DirectX;


#include <vector>
#include <list>
#include <unordered_map>
#include <map>
#include <algorithm>

#include "Engine_Typedef.h"
using namespace Engine;

#include "Engine_Macro.h"
#include "Engine_Struct.h"
#include "Engine_Function.h"

#ifdef _DEBUG

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#ifndef DBG_NEW
#define DBG_NEW new ( _NORMAL_BLOCK, __FILE__, __LINE__ )
#define new DBG_NEW
#endif

#endif // !_DEBUG
