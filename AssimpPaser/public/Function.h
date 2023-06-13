#pragma once

template <typename T>
void MySafe_Delete(T& pointer)
{
	if (nullptr != pointer)
	{
		delete pointer;
		pointer = nullptr;
	}
}

static HRESULT WriteTCHAR(HANDLE hFile, const _tchar* pBuffer, DWORD& dwByte)
{
	return WriteFile(hFile, pBuffer, lstrlen(pBuffer) * 2, &dwByte, nullptr);
}
//#define WriteTCHAR(hFile, pBuffer, dwByte) FAILED_CHECK_RETURN(WriteTCHAR(hFile, pBuffer, dwByte), E_FAIL)

static HRESULT WriteCHAR(HANDLE hFile, const char* pBuffer, DWORD& dwByte)
{
	char Buffer[MAX_PATH];
	_tchar LBuffer[MAX_PATH];
	strcpy_s(Buffer, pBuffer);
	MultiByteToWideChar(CP_ACP, 0, Buffer, MAX_PATH, LBuffer, MAX_PATH);
	return WriteFile(hFile, LBuffer, strlen(pBuffer) * 2, &dwByte, nullptr);
}
//#define WriteCHAR(hFile, pBuffer, dwByte) FAILED_CHECK_RETURN(WriteCHAR(hFile, pBuffer, dwByte), E_FAIL)

