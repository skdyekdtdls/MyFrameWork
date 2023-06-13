#pragma once

static bool WriteTCHAR(HANDLE hFile, const _tchar* pBuffer, DWORD& dwByte)
{
	return WriteFile(hFile, pBuffer, lstrlen(pBuffer) * 2, &dwByte, nullptr);
}
//#define WriteTCHAR(hFile, pBuffer, dwByte) FAILED_CHECK_RETURN(WriteTCHAR(hFile, pBuffer, dwByte), E_FAIL)

static bool WriteCHAR(HANDLE hFile, const char* pBuffer, DWORD& dwByte)
{
	char Buffer[MAX_PATH];
	_tchar LBuffer[MAX_PATH];
	strcpy_s(Buffer, pBuffer);
	MultiByteToWideChar(CP_ACP, 0, Buffer, MAX_PATH, LBuffer, MAX_PATH);
	return WriteFile(hFile, LBuffer, strlen(pBuffer) * 2, &dwByte, nullptr);
}
#define WriteCHAR(pBuffer) WriteCHAR(hFile, pBuffer, dwByte);

static bool WriteUINT(HANDLE hFile, _uint iBuffer, DWORD& dwByte)
{
	return WriteFile(hFile, &iBuffer, sizeof(_uint), &dwByte, nullptr);
}
#define WriteUINT(iBuffer) WriteUINT(hFile, iBuffer, dwByte)

static bool WriteAiVector3D(HANDLE hFile, aiVector3D pVBuffer, DWORD& dwByte)
{
	return WriteFile(hFile, &pVBuffer, sizeof(_float3), &dwByte, nullptr);
}
#define WriteAiVector3D(arg) WriteAiVector3D(hFile, arg, dwByte)

static bool ReadFloat3(HANDLE hFile, _float3& pVBuffer, DWORD& dwByte)
{
	return ReadFile(hFile, &pVBuffer, sizeof(_float3), &dwByte, nullptr);
}
#define ReadFloat3(arg) ReadFloat3(hFile, arg, dwByte)

static bool WriteAiVector2D(HANDLE hFile, aiVector2D pVBuffer, DWORD& dwByte)
{
	return WriteFile(hFile, &pVBuffer, sizeof(_float2), &dwByte, nullptr);
}
#define WriteAiVector2D(arg) WriteAiVector2D(hFile, arg, dwByte)

static bool ReadFloat2(HANDLE hFile, _float2& pVBuffer, DWORD& dwByte)
{
	return ReadFile(hFile, &pVBuffer, sizeof(_float2), &dwByte, nullptr);
}
#define ReadFloat2(arg) ReadFloat2(hFile, arg, dwByte)

static bool WriteAiMatrix4x4(HANDLE hFile, aiMatrix4x4 MatrixBuffer, DWORD& dwByte)
{
	return WriteFile(hFile, &MatrixBuffer, sizeof(_float4x4), &dwByte, nullptr);
}
#define WriteAiMatrix4x4(arg) WriteAiMatrix4x4(hFile, arg, dwByte)

static bool ReadFloat4x4(HANDLE hFile, _float4x4 MatrixBuffer, DWORD& dwByte)
{
	return ReadFile(hFile, &MatrixBuffer, sizeof(_float4x4), &dwByte, nullptr);
}
#define ReadFloat4x4(arg) ReadFloat4x4(hFile, arg, dwByte)