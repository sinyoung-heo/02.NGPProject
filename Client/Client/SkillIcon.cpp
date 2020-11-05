#include "stdafx.h"
#include "SkillIcon.h"


CSkillIcon::CSkillIcon()
{
}

CSkillIcon::~CSkillIcon()
{
	Release();
}

void CSkillIcon::Initialize()
{
	//m_eLayer = Layer::UI;

	m_tInfo.fCX = 24.f;
	m_tInfo.fCY = 24.f;

	m_iImageKey = 0;
}

void CSkillIcon::LateInit()
{

}

int CSkillIcon::Update()
{
	CObj::LateInit();
	if (m_bIsDead)
		return DEAD_OBJ;



	return NO_EVENT;
}

void CSkillIcon::LateUpdate()
{

}

void CSkillIcon::Render(HDC hDC)
{
	CObj::UpdateRect();

	Ellipse(hDC, m_tRect.left, m_tRect.top, m_tRect.right, m_tRect.bottom);

	HDC hMemDC = nullptr;
	NULL_CHECK(hMemDC = CBmpMgr::GetInstance()->FindImage(L"Quick_Skill"));

	GdiTransparentBlt(hDC, m_tRect.left, m_tRect.top, (int)m_tInfo.fCX, (int)m_tInfo.fCY, hMemDC,
		m_iImageKey*(int)m_tInfo.fCX, 0, (int)m_tInfo.fCX, (int)m_tInfo.fCY, RGB(0, 0, 0));
}

void CSkillIcon::Release()
{
}
