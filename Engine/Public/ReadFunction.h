#pragma once

namespace Engine
{
	static bool WriteEnable(HANDLE hFile, bool bValue, DWORD& dwByte)
	{
		return WriteFile(hFile, &bValue, sizeof(bool), &dwByte, nullptr);;
	}
#define WriteEnable(arg) Engine::WriteEnable(hFile, arg, dwByte)

	// void
	static bool WriteVoid(HANDLE hFile, const void* pBuffer, _uint iSize, DWORD& dwByte)
	{
		return WriteFile(hFile, pBuffer, iSize, &dwByte, nullptr);
	}
#define WriteVoid(arg, size) BOOL_CHECK(Engine::WriteVoid(hFile, arg, size, dwByte))
	///////////////////////////////////////////////////////////

	static bool ReadEnable(HANDLE hFile, DWORD& dwByte)
	{
		bool bValue = { false };
		ReadFile(hFile, &bValue, sizeof(bool), &dwByte, nullptr);
		return bValue;
	}
#define ReadEnable() Engine::ReadEnable(hFile, dwByte)

	// void
	static bool ReadVoid(HANDLE hFile, void* pBuffer, _uint iSize, DWORD& dwByte)
	{
		return ReadFile(hFile, pBuffer, iSize, &dwByte, nullptr);
	}
#define ReadVoid(arg, size) Engine::ReadVoid(hFile, arg, size, dwByte)
}


