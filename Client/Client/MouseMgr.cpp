#include "stdafx.h"
#include "MouseMgr.h"

IMPLEMENT_SINGLETON(CMouseMgr)

CMouseMgr::CMouseMgr()
	:m_eCurStance(END), m_ePreStance(END)
{
	ZeroMemory(&m_tCollInfo, sizeof(COLLINFO));
	ZeroMemory(&m_tCollRect, sizeof(RECT));
	ZeroMemory(&m_tFrame, sizeof(FRAME));
}


CMouseMgr::~CMouseMgr()
{
	Release();
}

void CMouseMgr::Initialize()
{
	CBmpMgr::GetInstance()->InsertBmp(L"Mouse", L"../Image/Ui/Mouse.bmp");
	CBmpMgr::GetInstance()->InsertBmp(L"Mouse_Attack", L"../Image/Ui/Mouse_Attack.bmp");
	CBmpMgr::GetInstance()->InsertBmp(L"Mouse_Fail", L"../Image/Ui/Mouse_Fail.bmp");

	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 13;
	m_tFrame.iScene = 0;
	m_tFrame.dwOldTime = GetTickCount();
	m_tFrame.dwFrameSpd = 200;

	m_pFrameKey = L"Mouse";
	m_eCurStance = IDLE;
	m_ePreStance = m_eCurStance;

	m_tCollInfo.fCX = 32.f;
	m_tCollInfo.fCY = 32.f;


	m_BlendFunction.BlendOp = 0;
	m_BlendFunction.BlendFlags = 0;
	m_BlendFunction.SourceConstantAlpha = 255;
	m_BlendFunction.AlphaFormat = AC_SRC_ALPHA;
}

void CMouseMgr::Update()
{
	POINT pt = {};
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);

	/*pt.x -= (int)CScrollMgr::GetScrollX();
	pt.y -= (int)CScrollMgr::GetScrollY();*/

	

	//UpdateColRect();
	m_tCollInfo.fX = (float)pt.x;
	m_tCollInfo.fY = (float)pt.y;
 
}

void CMouseMgr::LateUpdate()
{
	MoveFrame();
	SceneChange();
}

void CMouseMgr::Render(HDC hDC)
{
	UpdateColRect();

	HDC hMemDC = nullptr;
	NULL_CHECK(hMemDC = CBmpMgr::GetInstance()->FindImage(m_pFrameKey.c_str()));

	AlphaBlend(hDC, m_tCollInfo.fX, m_tCollInfo.fY, (int)m_tCollInfo.fCX, (int)m_tCollInfo.fCY,
		hMemDC, m_tFrame.iFrameStart * (int)m_tCollInfo.fCX, m_tFrame.iScene * (int)m_tCollInfo.fCY,
		(int)m_tCollInfo.fCX, (int)m_tCollInfo.fCY, m_BlendFunction);
}

void CMouseMgr::Release()
{
}

void CMouseMgr::MoveFrame()
{
	if (m_tFrame.dwOldTime + m_tFrame.dwFrameSpd < GetTickCount())
	{
		m_tFrame.iFrameStart++;
		m_tFrame.dwOldTime = GetTickCount();
	}

	if (m_tFrame.iFrameStart >= m_tFrame.iFrameEnd)
		m_tFrame.iFrameStart = 0;
}

void CMouseMgr::SceneChange()
{
	if (m_eCurStance != m_ePreStance)
	{
		switch (m_eCurStance)
		{
		case IDLE:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 12;
			m_tFrame.iScene = 0;
			m_tFrame.dwOldTime = GetTickCount();
			m_tFrame.dwFrameSpd = 200;
			break;
		case LCLICK:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 2;
			m_tFrame.iScene = 1;
			m_tFrame.dwOldTime = GetTickCount();
			m_tFrame.dwFrameSpd = 300;
			break;
		case ATTACK:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 6;
			m_tFrame.iScene = 0;
			m_tFrame.dwOldTime = GetTickCount();
			m_tFrame.dwFrameSpd = 200;
			break;
		case FAIL:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 11;
			m_tFrame.iScene = 0;
			m_tFrame.dwOldTime = GetTickCount();
			m_tFrame.dwFrameSpd = 200;
			break;
		}

		m_ePreStance = m_eCurStance;
	}
}

void CMouseMgr::UpdateColRect()
{
	m_tCollRect.left	= LONG(m_tCollInfo.fX - m_tCollInfo.fCX * 0.5f);
	m_tCollRect.top		= LONG(m_tCollInfo.fY - m_tCollInfo.fCY * 0.5f);
	m_tCollRect.right	= LONG(m_tCollInfo.fX + m_tCollInfo.fCX * 0.5f);
	m_tCollRect.bottom	= LONG(m_tCollInfo.fY + m_tCollInfo.fCY * 0.5f);
}

