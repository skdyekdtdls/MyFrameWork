#pragma once


#include <string>

#include <iostream>
#include <Windows.h>
#include <tchar.h>
#include <algorithm>
#include <regex>
#include <d3d11.h>
#include <DirectXMath.h>

#include "Assimp_Struct.h"
#include "Assimp_Typdef.h"


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

