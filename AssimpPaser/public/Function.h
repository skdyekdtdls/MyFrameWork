#pragma once

static bool WriteTCHAR(HANDLE hFile, const _tchar* pBuffer, DWORD& dwByte)
{
	return WriteFile(hFile, pBuffer, sizeof(_tchar) * MAX_PATH * 2, &dwByte, nullptr);
}
#define WriteTCHAR(pBuffer) WriteTCHAR(hFile, pBuffer, dwByte)


// char*
static bool WriteCHAR(HANDLE hFile, const char* pBuffer, DWORD& dwByte)
{
	char Buffer[MAX_PATH];
	_tchar LBuffer[MAX_PATH];
	strcpy_s(Buffer, pBuffer);
	MultiByteToWideChar(CP_ACP, 0, Buffer, MAX_PATH, LBuffer, MAX_PATH);
	return WriteFile(hFile, LBuffer, sizeof(_tchar) * MAX_PATH, &dwByte, nullptr);
}
#define WriteCHAR(pBuffer) WriteCHAR(hFile, pBuffer, dwByte)

// _uint
static bool WriteUINT(HANDLE hFile, _uint iBuffer, DWORD& dwByte)
{
	return WriteFile(hFile, &iBuffer, sizeof(_uint), &dwByte, nullptr);
}
#define WriteUINT(iBuffer) WriteUINT(hFile, iBuffer, dwByte)

// float3
static bool WriteAiVector3D(HANDLE hFile, aiVector3D pVBuffer, DWORD& dwByte)
{
	return WriteFile(hFile, &pVBuffer, sizeof(_float3), &dwByte, nullptr);
}
#define WriteAiVector3D(arg) WriteAiVector3D(hFile, arg, dwByte)

// float
static bool WriteFloat(HANDLE hFile, _float pVBuffer, DWORD& dwByte)
{
	return WriteFile(hFile, &pVBuffer, sizeof(_float), &dwByte, nullptr);
}
#define WriteFloat(arg) WriteFloat(hFile, arg, dwByte)

// _float2
static bool WriteAiVector2D(HANDLE hFile, aiVector2D pVBuffer, DWORD& dwByte)
{
	return WriteFile(hFile, &pVBuffer, sizeof(_float2), &dwByte, nullptr);
}
#define WriteAiVector2D(arg) WriteAiVector2D(hFile, arg, dwByte)

// _float4x4
static bool WriteAiMatrix4x4(HANDLE hFile, aiMatrix4x4 MatrixBuffer, DWORD& dwByte)
{
	return WriteFile(hFile, &MatrixBuffer, sizeof(_float4x4), &dwByte, nullptr);
}
#define WriteAiMatrix4x4(arg) WriteAiMatrix4x4(hFile, arg, dwByte)


