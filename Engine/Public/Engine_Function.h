#pragma once

template <typename T> // 클래스 포인터들을 해제
unsigned long Safe_AddRef(T& pointer)
{
	unsigned long dwRefCnt = { 0 };
	if (nullptr != pointer)
	{
		dwRefCnt = pointer->AddRef();
	}

	return dwRefCnt;
}

template <typename T>
unsigned long Safe_Release(T& pointer)
{
	unsigned long dwRefCnt = 0;
	if (nullptr != pointer)
	{
		dwRefCnt = pointer->Release();

		if (0 == dwRefCnt)
		{
			pointer = nullptr;
		}
	}

	return dwRefCnt;
}

template <typename T>
void Safe_Delete(T& pointer)
{
	if (nullptr != pointer)
	{
		delete pointer;
		pointer = nullptr;
	}
}

template <typename T>
void Safe_Delete_Array(T& pointer)
{
	if (nullptr != pointer)
	{
		delete[] pointer;
		pointer = nullptr;
	}
}

class CTag_Finder
{
public:
	CTag_Finder(const wchar_t* pTag)
		: m_pTargetTag(pTag){}
	~CTag_Finder() = default;

	template<typename T>
	bool operator()(const T& pair)
	{
		return 0 == lstrcmpW(m_pTargetTag, pair.first);
	}

private:
	const wchar_t* m_pTargetTag = { nullptr };
};

#include <locale>
#include <codecvt>

static string wstrToStr(const wstring& wstr)
{
	int size_needed = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), (int)wstr.size(), NULL, 0, NULL, NULL);
	std::string str_to(size_needed, 0);
	WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), (int)wstr.size(), &str_to[0], size_needed, NULL, NULL);
	return str_to;
}
#define TO_STR(WSTR) wstrToStr(WSTR)

static wstring strToWStr(const string& str)
{
	int size_needed = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), (int)str.size(), NULL, 0);
	std::wstring wstr_to(size_needed, 0);
	MultiByteToWideChar(CP_UTF8, 0, str.c_str(), (int)str.size(), &wstr_to[0], size_needed);
	return wstr_to;
}

#define TO_WSTR(STR) strToWStr(STR)