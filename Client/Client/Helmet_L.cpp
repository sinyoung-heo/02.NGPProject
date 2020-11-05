#include "stdafx.h"
#include "Helmet_L.h"


CHelmet_L::CHelmet_L()
{
}

CHelmet_L::~CHelmet_L()
{
	Release();

}

void CHelmet_L::Initialize()
{
	CBmpMgr::GetInstance()->InsertBmp(L"head_low", L"../Image/item/head_low.bmp");
	m_pImageKey = L"head_low";

	CItem::Initialize();

	m_eItemType = ItemType::HELMET;

	m_tInfo.szName = L"head_low";
	m_tInfo.iMaxMp = 1000;
	m_tInfo.iGold = 5000;
}

void CHelmet_L::LateInit()
{
}

int CHelmet_L::Update()
{
	CObj::LateInit();
	if (m_bIsDead)
		return DEAD_OBJ;

	return NO_EVENT;
}

void CHelmet_L::LateUpdate()
{
}

void CHelmet_L::Render(HDC hDC)
{
	//CObj::UpdateRect();

	//HDC hMemDC = nullptr;
	//NULL_CHECK(hMemDC = CBmpMgr::GetInstance()->FindImage(L"head_low"));

	//GdiTransparentBlt(hDC, m_tRect.left, m_tRect.top, (int)m_tInfo.fX, (int)m_tInfo.fY,
	//	hMemDC, 0, 0, (int)m_tInfo.fCX, (int)m_tInfo.fCY, RGB(255, 0, 255));
}

void CHelmet_L::Release()
{
}
