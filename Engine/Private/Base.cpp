#include "Base.h"


_ulong CBase::Add_Ref()
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
