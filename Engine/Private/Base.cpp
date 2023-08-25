#include "Base.h"


_ulong CBase::AddRef()
{
	return ++m_dwRefCnt;
}

_ulong CBase::Release()
{
	if (0 == m_dwRefCnt)
	{
		Free();

		delete this;
		return 0;
	}

	return m_dwRefCnt--;
}
#ifdef _DEBUG
void CBase::CoutRefCnt()
{
	cout << m_dwRefCnt << endl;
}
#endif