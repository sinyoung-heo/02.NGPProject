#include "stdafx.h"
#include "PlayerCrash.h"


CPlayerCrash::CPlayerCrash()
{
}

CPlayerCrash::~CPlayerCrash()
{
	Release();
}

void CPlayerCrash::Initialize()
{
	m_eLayer = Layer::EFFECT;

	m_tInfo.fCX = 127.f;
	m_tInfo.fCY = 123.f;

	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 3;
	m_tFrame.dwFrameSpd = 80;
	m_tFrame.dwOldTime = GetTickCount();
}

void CPlayerCrash::LateInit()
{
	CSoundMgr::GetInstance()->PlaySound(L"Hit.wav", CSoundMgr::ATTACK_EFFECT);

}

int CPlayerCrash::Update()
{
	CObj::LateInit();
	if (m_bIsDead)
		return DEAD_OBJ;



	return NO_EVENT;
}

void CPlayerCrash::LateUpdate()
{
	if (m_tFrame.iFrameStart == m_tFrame.iFrameEnd)
		m_bIsDead = true;

	CObj::MoveFrame();
}

void CPlayerCrash::Render(HDC hDC)
{
	CObj::UpdateRect();

	HDC hMemDC = nullptr;
	NULL_CHECK(hMemDC = CBmpMgr::GetInstance()->FindImage(L"Crash"));
	
	float fScrollX = CScrollMgr::GetScrollX();
	float fScrollY = CScrollMgr::GetScrollY();

	GdiTransparentBlt(hDC, m_tRect.left + (int)fScrollX, m_tRect.top + (int)fScrollY, (int)m_tInfo.fCX, (int)m_tInfo.fCY,
		hMemDC, m_tFrame.iFrameStart * (int)m_tInfo.fCX, 0, 
		(int)m_tInfo.fCX, (int)m_tInfo.fCY, RGB(255, 0, 255));

}

void CPlayerCrash::Release()
{
}
