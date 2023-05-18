#pragma once

template <typename T> // 클래스 포인터들을 해제
unsigned long Safe_AddRef(T& pointer)
{
	unsigned long dwRefCnt = { 0 };
	if (nullptr != pointer)
	{
		dwRefCnt = pointer->Add_Ref();
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