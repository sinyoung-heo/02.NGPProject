#include "stdafx.h"
#include "LevelUp.h"


CLevelUp::CLevelUp()
{
}

CLevelUp::~CLevelUp()
{
	Release();
}

void CLevelUp::Initialize()
{
	m_eLayer = Layer::EFFECT;

	m_tInfo.fCX = 256.f;
	m_tInfo.fCY = 256.f;

	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 29;
	m_tFrame.dwFrameSpd = 10;
	m_tFrame.dwOldTime = GetTickCount();
}

void CLevelUp::LateInit()
{
}

int CLevelUp::Update()
{
	CObj::LateInit();
	if (m_bIsDead)
		return DEAD_OBJ;



	return NO_EVENT;
}

void CLevelUp::LateUpdate()
{

	if (m_tFrame.iFrameStart == m_tFrame.iFrameEnd)
		m_bIsDead = true;

	CObj::MoveFrame();
}

void CLevelUp::Render(HDC hDC)
{
	CObj::UpdateRect();

	HDC hMemDC = nullptr;
	NULL_CHECK(hMemDC = CBmpMgr::GetInstance()->FindImage(L"LvUp"));

	float fScrollX = CScrollMgr::GetScrollX();
	float fScrollY = CScrollMgr::GetScrollY();

	GdiTransparentBlt(hDC, m_tRect.left + (int)fScrollX, m_tRect.top + (int)fScrollY, (int)m_tInfo.fCX, (int)m_tInfo.fCY,
		hMemDC, m_tFrame.iFrameStart * (int)m_tInfo.fCX, 0,
		(int)m_tInfo.fCX, (int)m_tInfo.fCY, RGB(255, 0, 255));
}

void CLevelUp::Release()
{
}
