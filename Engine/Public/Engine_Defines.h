#pragma once

#pragma warning (disable : 4005)

#include "assimp\Importer.hpp"
#include "assimp\scene.h"
#include "assimp\postprocess.h"
#include <iostream>
#include <tchar.h>
#include <DirectXMath.h>
#include <d3dcompiler.h>
#include "fx11\d3dx11effect.h"
#include <cassert>
#include <DirectXCollision.h>
#include "DirectXTK\ScreenGrab.h"

#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

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
#include "ReadFunction.h"
#include "Engine_Struct.h"
#include "Engine_Function.h"
#include "DirectXTK/DDSTextureLoader.h"
#include "DirectXTK/WICTextureLoader.h"
#include "Assimp_Struct.h"

#ifdef _DEBUG

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#ifndef DBG_NEW 

#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ ) 

#ifndef _USE_IMGUI
#define new DBG_NEW 
#else
#define New DBG_NEW
#endif // _USE_IMGUI

#endif // _DEBUG
#endif // _DEBUG
