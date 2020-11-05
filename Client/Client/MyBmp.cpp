#include "stdafx.h"
#include "MyBmp.h"


CMyBmp::CMyBmp()
{
}

CMyBmp::~CMyBmp()
{
	Release();
}

void CMyBmp::LoadBmp(wstring pFilePath)
{
	m_hBitmap = (HBITMAP)LoadImage(nullptr, pFilePath.c_str(), IMAGE_BITMAP, 0, 0,
		LR_LOADFROMFILE | LR_CREATEDIBSECTION);

	HDC hDC = GetDC(g_hWnd);

	m_hMemDC = CreateCompatibleDC(hDC);
	ReleaseDC(g_hWnd, hDC);

	m_hOld = (HBITMAP)SelectObject(m_hMemDC, m_hBitmap);
}

void CMyBmp::Release()
{
	SelectObject(m_hMemDC, m_hOld);
	DeleteObject(m_hBitmap);
	DeleteDC(m_hMemDC);
}
