#include "stdafx.h"
#include "Seller.h"
#include "SellLst.h"


CSeller::CSeller()
	:m_bIsSellLstOn(false)
{

}

CSeller::~CSeller()
{
	Release();
}

void CSeller::Initialize()
{
	CBmpMgr::GetInstance()->InsertBmp(L"Linda", L"../Image/Npc/Linda.bmp");

	m_tInfo.fCX = 128.f;
	m_tInfo.fCY = 128.f;

	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 11;
	m_tFrame.iScene = 0;
	m_tFrame.dwOldTime = GetTickCount();
	m_tFrame.dwFrameSpd = 200;

	m_pSellLst = CObjMgr::GetInstance()->GetSellLst();
}

void CSeller::LateInit()
{

}

int CSeller::Update()
{
	CObj::LateInit();
	if (m_bIsDead)
		return DEAD_OBJ;

	POINT pt = {};
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);

	float fScrollX = CScrollMgr::GetScrollX();
	float fScrollY = CScrollMgr::GetScrollY();

	pt.x -= (LONG)fScrollX;
	pt.y -= (LONG)fScrollY;
	
	CObj::UpdateRect();
	
	if (PtInRect(&m_tRect, pt))
	{
		if (KEY_DOWN(VK_LBUTTON))
		{

			m_bIsSellLstOn = !m_bIsSellLstOn;
			static_cast<CSellLst*>(m_pSellLst)->SetbIsSellLst(m_bIsSellLstOn);
		}
	}


	return NO_EVENT;
}

void CSeller::LateUpdate()
{
	CObj::MoveFrame();
}

void CSeller::Render(HDC hDC)
{
	CObj::UpdateRect();

	HDC hMemDC = nullptr;
	NULL_CHECK(hMemDC = CBmpMgr::GetInstance()->FindImage(L"Linda"));

	float fScrollX = CScrollMgr::GetScrollX();
	float fScrollY = CScrollMgr::GetScrollY();

	GdiTransparentBlt(hDC, m_tRect.left + (int)fScrollX, m_tRect.top + (int)fScrollY, (int)m_tInfo.fCX, (int)m_tInfo.fCY,
		hMemDC, m_tFrame.iFrameStart*(int)m_tInfo.fCX, 0, (int)m_tInfo.fCX, (int)m_tInfo.fCY, RGB(0, 0, 0));
}

void CSeller::Release()
{

}
