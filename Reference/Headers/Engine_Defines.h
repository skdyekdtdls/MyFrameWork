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
#include "DirectXTK/PrimitiveBatch.h"
#include "DirectXTK/VertexTypes.h"
#include "DirectXTK/Effects.h"
#include "DirectXTK/SpriteBatch.h"
#include "DirectXTK/SpriteFont.h"



#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

using namespace std;
using namespace DirectX;
#include <string>
#include <vector>
#include <list>
#include <unordered_map>
#include <map>
#include <algorithm>
#include <filesystem>
#include <queue>
#include "Engine_Typedef.h"
using namespace Engine;
namespace fs = std::filesystem;

#include "Engine_Function.h"
#include "Engine_Macro.h"
#include "ReadFunction.h"
#include "Engine_Struct.h"

BEGIN(Engine)
typedef unordered_map<const _tchar*, class CLayer*> LAYERS;
typedef pair <_double, function<void()> > TIMELINE_EVENT;
END

#include "DirectXTK/DDSTextureLoader.h"
#include "DirectXTK/WICTextureLoader.h"
#include "Assimp_Struct.h"

enum BODY { LOWER, UPPER, BODY_END };

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
