#include "stdafx.h"
#include "MonCrash.h"


CMonCrash::CMonCrash()
{
}

CMonCrash::~CMonCrash()
{
	Release();
}

void CMonCrash::Initialize()
{
	m_eLayer = Layer::EFFECT;

	m_tInfo.fCX = 127.f;
	m_tInfo.fCY = 123.f;

	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 3;
	m_tFrame.dwFrameSpd = 80;
	m_tFrame.dwOldTime = GetTickCount();
}

void CMonCrash::LateInit()
{
}

int CMonCrash::Update()
{
	CObj::LateInit();
	if (m_bIsDead)
		return DEAD_OBJ;



	return NO_EVENT;
}

void CMonCrash::LateUpdate()
{
	if (m_tFrame.iFrameStart == m_tFrame.iFrameEnd)
		m_bIsDead = true;

	CObj::MoveFrame();
}


void CMonCrash::Render(HDC hDC)
{
	CObj::UpdateRect();

	HDC hMemDC = nullptr;
	NULL_CHECK(hMemDC = CBmpMgr::GetInstance()->FindImage(L"Crash2"));

	float fScrollX = CScrollMgr::GetScrollX();
	float fScrollY = CScrollMgr::GetScrollY();

	GdiTransparentBlt(hDC, m_tRect.left + (int)fScrollX, m_tRect.top + (int)fScrollY, (int)m_tInfo.fCX, (int)m_tInfo.fCY,
		hMemDC, m_tFrame.iFrameStart * (int)m_tInfo.fCX, 0,
		(int)m_tInfo.fCX, (int)m_tInfo.fCY, RGB(255, 255, 255));
}

void CMonCrash::Release()
{
}
