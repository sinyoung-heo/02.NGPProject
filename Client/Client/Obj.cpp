#include "stdafx.h"
#include "Obj.h"

CObj::CObj()
	:m_fAngle(0.f), m_bLateInit(false), m_bIsDead(false), m_eLayer(Layer::GAMEOBJECT)
{
	ZeroMemory(&m_tInfo, sizeof(INFO));
	ZeroMemory(&m_tRect, sizeof(RECT));
	ZeroMemory(&m_tColInfo, sizeof(COLLINFO));
	ZeroMemory(&m_tColRect, sizeof(RECT));
	ZeroMemory(&m_tFrame, sizeof(FRAME));
}

CObj::CObj(const INFO & rInfo)
	:m_tInfo(rInfo)
{
}


CObj::~CObj()
{
}

void CObj::LateInit()
{
	if (!m_bLateInit)
	{
		this->LateInit();
		m_bLateInit = true;
	}
}

void CObj::UpdateRect()
{
	m_tRect.left   = LONG(m_tInfo.fX - m_tInfo.fCX * 0.5f);
	m_tRect.top    = LONG(m_tInfo.fY - m_tInfo.fCY * 0.5f);
	m_tRect.right  = LONG(m_tInfo.fX + m_tInfo.fCX * 0.5f);
	m_tRect.bottom = LONG(m_tInfo.fY + m_tInfo.fCY * 0.5f);
}

/*충돌박스 업데이트*/
void CObj::UpdateColRect()
{
	m_tColRect.left		= LONG(m_tColInfo.fX - m_tColInfo.fCX * 0.5f);
	m_tColRect.top		= LONG(m_tColInfo.fY - m_tColInfo.fCY * 0.5f);
	m_tColRect.right	= LONG(m_tColInfo.fX + m_tColInfo.fCX * 0.5f);
	m_tColRect.bottom	= LONG(m_tColInfo.fY + m_tColInfo.fCY * 0.5f);
}
/*충돌박스 X,Y좌표 업데이트*/
void CObj::UpdateColXYPos()
{
	m_tColInfo.fX = m_tInfo.fX;
	m_tColInfo.fY = m_tInfo.fY;
}

/*이미지 재생*/
void CObj::MoveFrame()
{
	if (m_tFrame.dwOldTime + m_tFrame.dwFrameSpd < GetTickCount())
	{
		m_tFrame.iFrameStart++;
		m_tFrame.dwOldTime = GetTickCount();
	}

	if (m_tFrame.iFrameStart > m_tFrame.iFrameEnd)
		m_tFrame.iFrameStart = 0;
}
