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


static std::string WideToMultiByte(const std::wstring& wide_str)
{
	int requiredSize = WideCharToMultiByte(CP_UTF8, 0, wide_str.c_str(), -1, NULL, 0, NULL, NULL);
	std::string multiByte_str(requiredSize, 0);
	WideCharToMultiByte(CP_UTF8, 0, wide_str.c_str(), -1, &multiByte_str[0], requiredSize, NULL, NULL);
	return multiByte_str;
}
#define TO_STR(WSTR) WideToMultiByte(WSTR)


std::wstring MultiByteToWide(const std::string& multiByte_str)
{
	int requiredSize = MultiByteToWideChar(CP_UTF8, 0, multiByte_str.c_str(), -1, NULL, 0);
	std::wstring wide_str(requiredSize, 0);
	MultiByteToWideChar(CP_UTF8, 0, multiByte_str.c_str(), -1, &wide_str[0], requiredSize);
	return wide_str;
}

#define TO_WSTR(STR) MultiByteToWide(STR)