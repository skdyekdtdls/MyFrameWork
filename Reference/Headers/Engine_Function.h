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


static std::wstring MultiByteToWide(const std::string& multiByte_str)
{
	int requiredSize = MultiByteToWideChar(CP_UTF8, 0, multiByte_str.c_str(), -1, NULL, 0);
	std::wstring wide_str(requiredSize, 0);
	MultiByteToWideChar(CP_UTF8, 0, multiByte_str.c_str(), -1, &wide_str[0], requiredSize);
	return wide_str;
}
#define TO_WSTR(STR) MultiByteToWide(STR)

static bool FloatEqual(float f1, float f2, float epsilon = std::numeric_limits<float>::epsilon())
{
	return abs(f1 - f2) <= epsilon;
}

static bool Float3Equal(_float3 f1, _float3 f2, float epsilon = std::numeric_limits<float>::epsilon())
{
	if (!FloatEqual(f1.x, f2.x, epsilon)) return false;
	if (!FloatEqual(f1.y, f2.y, epsilon)) return false;
	if (!FloatEqual(f1.z, f2.z, epsilon)) return false;
	return true;
}

bool IsPointOnLineSegment(FXMVECTOR A, FXMVECTOR B, FXMVECTOR P)
{
	DirectX::FXMVECTOR AB = DirectX::XMVectorSubtract(B, A);
	DirectX::FXMVECTOR AP = DirectX::XMVectorSubtract(P, A);
	DirectX::FXMVECTOR BP = DirectX::XMVectorSubtract(P, B);

	float dotABAP = DirectX::XMVectorGetX(DirectX::XMVector3Dot(AB, AP));
	float dotABAB = DirectX::XMVectorGetX(DirectX::XMVector3Dot(AB, AB));
	float dotBAPB = DirectX::XMVectorGetX(DirectX::XMVector3Dot(DirectX::XMVectorNegate(AB), BP));

	return (dotABAP >= 0.f && dotABAP <= dotABAB && dotBAPB >= 0.f);
}