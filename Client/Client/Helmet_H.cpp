#include "stdafx.h"
#include "Helmet_H.h"


CHelmet_H::CHelmet_H()
{

}

CHelmet_H::~CHelmet_H()
{
	Release();
}

void CHelmet_H::Initialize()
{
	CBmpMgr::GetInstance()->InsertBmp(L"head_high", L"../Image/item/head_high.bmp");
	m_pImageKey = L"head_high";

	CItem::Initialize();

	m_eItemType = ItemType::HELMET;
	
	m_tInfo.szName = L"head_high";
	m_tInfo.iMaxMp = 3000;
	m_tInfo.iGold = 10000;
}

void CHelmet_H::LateInit()
{

}

int CHelmet_H::Update()
{
	CObj::LateInit();
	if (m_bIsDead)
		return DEAD_OBJ;


	return NO_EVENT;
}

void CHelmet_H::LateUpdate()
{

}

void CHelmet_H::Render(HDC hDC)
{
	/*CObj::UpdateRect();

	HDC hMemDC = nullptr;
	NULL_CHECK(hMemDC = CBmpMgr::GetInstance()->FindImage(L"head_high"));

	GdiTransparentBlt(hDC, m_tRect.left, m_tRect.top, (int)m_tInfo.fX, (int)m_tInfo.fY,
		hMemDC, 0, 0, (int)m_tInfo.fCX, (int)m_tInfo.fCY, RGB(255, 0, 255));*/
}

void CHelmet_H::Release()
{

}
