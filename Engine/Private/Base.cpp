#include "..\Public\Base.h"

CBase::CBase()
{
	int			iData = 10;

}

unsigned long CBase::AddRef()
{
	return ++m_dwRefCnt;	
}

unsigned long CBase::Release()
{
	/* 삭제한다. */
	if (0 == m_dwRefCnt)
	{
		Free();

		delete this;

		return 0;
	}
	else
	{
		/* 감소한다. */
		return m_dwRefCnt--;
	}
}

void CBase::Free()
{
}
