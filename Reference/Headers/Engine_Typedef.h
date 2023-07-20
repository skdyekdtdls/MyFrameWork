#pragma once

namespace Engine
{
	typedef bool _bool;

	typedef char	_byte;
	typedef unsigned char _ubyte;
	typedef char	_char;
	typedef unsigned char _uchar;

	typedef wchar_t	_tchar;

	typedef signed short _short;
	typedef unsigned short _ushort;

	typedef signed int _int;
	typedef unsigned int _uint;

	typedef signed long _long;
	typedef unsigned long _ulong;

	typedef float _float;
	typedef double _double;

	typedef XMUINT2		_uint2;

	typedef XMFLOAT2	_float2;
	typedef XMFLOAT3	_float3;
	typedef XMFLOAT4	_float4;
	typedef XMFLOAT4X4	_float4x4;

	typedef XMVECTOR	_vector;
	typedef FXMVECTOR	_fvector; // 1, 2, 3
	typedef GXMVECTOR	_gvector; // 4
	typedef HXMVECTOR	_hvector; // 5, 6
	typedef CXMVECTOR	_cvector; // 7+

	typedef XMMATRIX	_matrix;
	typedef FXMMATRIX	_fmatrix;
}