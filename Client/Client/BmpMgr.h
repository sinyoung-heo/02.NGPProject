#pragma once

class CMyBmp;
class CBmpMgr
{
	DECLARE_SINGLETON(CBmpMgr)

private:
	CBmpMgr();
	~CBmpMgr();

public:
	HDC FindImage(wstring pImageKey);

public:
	void InsertBmp(wstring pImageKey, wstring pFilePath);
	void Release();
private:
	map<wstring, CMyBmp*>	m_MapBit;

};

