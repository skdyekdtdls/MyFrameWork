#pragma once
#pragma warning (disable : 4005)

#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include <tchar.h>

#include <iostream>
#include <tchar.h>
#include <DirectXMath.h>
#include <d3dcompiler.h>
#include "fx11\d3dx11effect.h"
#include <cassert>

using namespace std;
using namespace DirectX;

#include "Imgui_Function.h"
#include "Imgui_Macro.h"
#include "Imgui_Typedef.h"
#include "Imgui_Struct.h"

#include <vector>
#include <list>
#include <unordered_map>
#include <map>
#include <algorithm>

#include "DirectXTK/DDSTextureLoader.h"
#include "DirectXTK/WICTextureLoader.h"

#ifdef _DEBUG

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#ifndef DBG_NEW 

#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ ) 

#define New DBG_NEW

#endif // _DEBUG
#endif // _DEBUG
