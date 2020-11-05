#pragma once

class CMyBmp
{
public:
	CMyBmp();
	~CMyBmp();

public:
	HDC GetMemDC() { return m_hMemDC; }

public:
	void LoadBmp(wstring pFilePath);
	void Release();

private:
	HDC		m_hMemDC;

	HBITMAP	m_hBitmap;
	HBITMAP	m_hOld;
};

