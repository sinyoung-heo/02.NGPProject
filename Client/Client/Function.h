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

/*���� ������ ���� ���� �Լ�*/
template <typename T>
bool LessY(T pDst, T pSrc)
{
	return pDst->GetInfo().fY < pSrc->GetInfo().fY;
}
