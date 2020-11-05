#include "stdafx.h"
#include "BmpMgr.h"
#include "MyBmp.h"

IMPLEMENT_SINGLETON(CBmpMgr)

CBmpMgr::CBmpMgr()
{
}

CBmpMgr::~CBmpMgr()
{
	Release();
}

HDC CBmpMgr::FindImage(wstring pImageKey)
{
	if (m_MapBit.empty())
		return nullptr;

	auto iter_find = m_MapBit.find(pImageKey);

	if (m_MapBit.end() == iter_find)
		return nullptr;

	return iter_find->second->GetMemDC();
}

void CBmpMgr::InsertBmp(wstring pImageKey, wstring pFilePath)
{
	if (true == m_MapBit.insert({ pImageKey, nullptr }).second)
	{
		m_MapBit[pImageKey] = new CMyBmp;
		m_MapBit[pImageKey]->LoadBmp(pFilePath.c_str());
	}
}

void CBmpMgr::Release()
{
	for_each(m_MapBit.begin(), m_MapBit.end(),
		[](auto& MyPair)
	{
		SafeDelete(MyPair.second);
	});

	m_MapBit.clear();
}
