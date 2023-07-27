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

static bool DoubleEqual(double d1, double d2, double epsilon = std::numeric_limits<double>::epsilon())
{
	return abs(d1 - d2) <= epsilon;
}

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

template <typename T>
static bool	isInsideRange(T _value, T _min, T _max)
{
	if (_min <= _value && _value <= _max)
		return true;

	return false;
}

// 값을 최소 최대 사이로 자르고 반환함.
template <typename T>
static void Saturate(T& _value, T _min, T _max)
{
	_value = max(_value, _min);
	_value = min(_value, _max);
}

// 두 벡터 사이의 Radian을 구하는 함수
static float RadianBetweenVectors(FXMVECTOR _v1, FXMVECTOR _v2)
{
	XMVECTOR v1 = XMVector3Normalize(_v1);
	XMVECTOR v2 = XMVector3Normalize(_v2);

	float fDot = XMVectorGetX(XMVector3Dot(v1, v2));
	Saturate(fDot, -1.f, 1.f);
	XMVECTOR cross = XMVector3Cross(_v1, _v2);
	float crossY = XMVectorGetY(cross); // Z 값을 얻습니다. Z 축은 두 벡터의 수평면에 수직입니다.
	float radian = acos(fDot);

	if (0 < crossY)
	{
		radian = (2 * XM_PI) - radian; // 360도 - 계산된 각도
	}

	return radian;
}

static int RandomIntFrom_A_To_B(int A, int B)
{
	if (A == 0 && B == 0)
		return 0;

	if (A > B)
		std::swap(A, B);

	return rand() % (B - A + 1) + A;
}

// 두 벡터 사이의 Degree를 구하는 함수.
static float DegreeBetweenVectors(FXMVECTOR _v1, FXMVECTOR _v2)
{
	return XMConvertToDegrees(RadianBetweenVectors(_v1, _v2));
}

// 각도를 0~360도로 조정해주는 함수.
static void DegreeClipping(_float& fDegree)
{
	if (fDegree > 0)
	{
		// 나머지연산(fmod는 float에 대한 나머지를 구하는 함수)
		fDegree = fmod(fDegree, 360.f);
	}
	else
	{
		// 각도가 0~360안에 들어갈 때 까지 루프를 돔.
		while (0 > fDegree)
			fDegree += 360.f;
	}	
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

static XMVECTOR WorldAxisX() { return XMVectorSet(1.f, 0.f, 0.f, 0.f); }
static XMVECTOR WorldAxisY() { return XMVectorSet(0.f, 1.f, 0.f, 0.f); }
static XMVECTOR WorldAxisZ() { return XMVectorSet(0.f, 0.f, 1.f, 0.f); }

#ifdef _DEBUG
static void CoutVector(_fvector vector)
{
	cout << vector.m128_f32[0] << "\t" << vector.m128_f32[1] << "\t" << vector.m128_f32[2] << endl;
}

static void CoutFloat3(_float3 vector)
{
	cout << vector.x << "\t" << vector.y << "\t" << vector.z << endl;
}
#endif