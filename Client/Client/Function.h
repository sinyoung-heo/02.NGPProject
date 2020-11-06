#pragma once

template <typename T>
void SafeDelete(T& ptr)
{
	if (ptr)
	{
		delete ptr;
		ptr = nullptr;
	}
}

/*랜더 소팅을 위한 정렬 함수*/
template <typename T>
bool LessY(T pDst, T pSrc)
{
	return pDst->GetInfo().fY < pSrc->GetInfo().fY;
}
