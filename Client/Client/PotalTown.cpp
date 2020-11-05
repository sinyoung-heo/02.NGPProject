#include "stdafx.h"
#include "PotalTown.h"


CPotalTown::CPotalTown()
{

}

CPotalTown::~CPotalTown()
{
	Release();
}

void CPotalTown::Initialize()
{
	m_eLayer = Layer::EFFECT;

	m_tInfo.fCX = 150.f;
	m_tInfo.fCY = 200.f;
	
	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 23;
	m_tFrame.dwFrameSpd = 40;
	m_tFrame.dwOldTime = GetTickCount();
	m_tFrame.iScene = 0;
}

void CPotalTown::LateInit()
{

}

int CPotalTown::Update()
{
	CObj::LateInit();
	if (m_bIsDead)
		return DEAD_OBJ;

	return NO_EVENT;
}

void CPotalTown::LateUpdate()
{
	CObj::MoveFrame();
}

void CPotalTown::Render(HDC hDC)
{
	CObj::UpdateRect();

	HDC hMemDC = nullptr;
	NULL_CHECK(hMemDC = CBmpMgr::GetInstance()->FindImage(L"Portal2"));

	float fScrollX = CScrollMgr::GetScrollX();
	float fScrollY = CScrollMgr::GetScrollY();

	GdiTransparentBlt(hDC, m_tRect.left + (int)fScrollX, m_tRect.top + (int)fScrollY, (int)m_tInfo.fCX, (int)m_tInfo.fCY,
		hMemDC, m_tFrame.iFrameStart*(int)m_tInfo.fCX, 0, (int)m_tInfo.fCX, (int)m_tInfo.fCY, RGB(255, 255, 255));
}

void CPotalTown::Release()
{

}
