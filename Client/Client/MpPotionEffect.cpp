#include "stdafx.h"
#include "MpPotionEffect.h"


CMpPotionEffect::CMpPotionEffect()
{
}

CMpPotionEffect::~CMpPotionEffect()
{
	Release();
}

void CMpPotionEffect::Initialize()
{
	m_eLayer = Layer::EFFECT;

	m_tInfo.fCX = 128.f;
	m_tInfo.fCY = 128.f;

	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 5;
	m_tFrame.iScene = 0;
	m_tFrame.dwOldTime = GetTickCount();
	m_tFrame.dwFrameSpd = 120;

	m_BlendFunction.BlendOp = 0;
	m_BlendFunction.BlendFlags = 0;
	m_BlendFunction.SourceConstantAlpha = 110;
	m_BlendFunction.AlphaFormat = AC_SRC_ALPHA;
}

void CMpPotionEffect::LateInit()
{

}

int CMpPotionEffect::Update()
{
	CObj::LateInit();
	if (m_bIsDead)
		return DEAD_OBJ;

	return NO_EVENT;
}

void CMpPotionEffect::LateUpdate()
{
	if (m_tFrame.iFrameStart == m_tFrame.iFrameEnd)
		m_bIsDead = true;

	CObj::MoveFrame();
}

void CMpPotionEffect::Render(HDC hDC)
{
	CObj::UpdateRect();

	HDC hMemDC = nullptr;
	NULL_CHECK(hMemDC = CBmpMgr::GetInstance()->FindImage(L"MpPotion"));

	float fScrollX = CScrollMgr::GetScrollX();
	float fScrollY = CScrollMgr::GetScrollY();

	AlphaBlend(hDC, m_tRect.left + (int)fScrollX, m_tRect.top + (int)fScrollY, (int)m_tInfo.fCX, (int)m_tInfo.fCY,
		hMemDC, m_tFrame.iFrameStart*(int)m_tInfo.fCX, 0, (int)m_tInfo.fCX, (int)m_tInfo.fCY, m_BlendFunction);
}

void CMpPotionEffect::Release()
{

}