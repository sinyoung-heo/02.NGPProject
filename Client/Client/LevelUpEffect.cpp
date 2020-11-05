#include "stdafx.h"
#include "LevelUpEffect.h"


CLevelUpEffect::CLevelUpEffect()
{
}

CLevelUpEffect::~CLevelUpEffect()
{
	Release();
}

void CLevelUpEffect::Initialize()
{
	m_eLayer = Layer::EFFECT;

	m_tInfo.fCX = 128.f;
	m_tInfo.fCY = 128.f;

	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 19;
	m_tFrame.dwFrameSpd = 30;
	m_tFrame.dwOldTime = GetTickCount();


	m_BlendFunction.BlendOp = 0;
	m_BlendFunction.BlendFlags = 0;
	m_BlendFunction.SourceConstantAlpha = 255;
	m_BlendFunction.AlphaFormat = AC_SRC_ALPHA;
}

void CLevelUpEffect::LateInit()
{

}

int CLevelUpEffect::Update()
{
	CObj::LateInit();
	if (m_bIsDead)
		return DEAD_OBJ;



	return NO_EVENT;
}

void CLevelUpEffect::LateUpdate()
{

	if (m_tFrame.iFrameStart == m_tFrame.iFrameEnd)
		m_bIsDead = true;

	CObj::MoveFrame();
}

void CLevelUpEffect::Render(HDC hDC)
{
	CObj::UpdateRect();

	HDC hMemDC = nullptr;
	NULL_CHECK(hMemDC = CBmpMgr::GetInstance()->FindImage(L"LvUpEffect"));

	float fScrollX = CScrollMgr::GetScrollX();
	float fScrollY = CScrollMgr::GetScrollY();

	AlphaBlend(hDC, m_tRect.left + (int)fScrollX, m_tRect.top + (int)fScrollY, (int)m_tInfo.fCX, (int)m_tInfo.fCY,
		hMemDC, m_tFrame.iFrameStart * (int)m_tInfo.fCX, 0,
		(int)m_tInfo.fCX, (int)m_tInfo.fCY, m_BlendFunction);
}

void CLevelUpEffect::Release()
{
}
