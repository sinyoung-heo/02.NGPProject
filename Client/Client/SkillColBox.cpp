#include "stdafx.h"
#include "SkillColBox.h"

static int iTime = 0;

CSkillColBox::CSkillColBox()
{
}

CSkillColBox::~CSkillColBox()
{
	Release();
}

void CSkillColBox::Initialize()
{

}

void CSkillColBox::LateInit()
{
	CObj::UpdateRect();
}

int CSkillColBox::Update()
{
	CObj::LateInit();
	if (m_bIsDead)
		return DEAD_OBJ;



	iTime += 5;

	return NO_EVENT;
}

void CSkillColBox::LateUpdate()
{
	/*스킬 재생시간.*/
	if (iTime > 100)
	{
		bIsDead();
		iTime = 0;
	}
}

void CSkillColBox::Render(HDC hDC)
{
	CObj::UpdateRect();

	/*float fScrollX = CScrollMgr::GetScrollX();
	float fScrollY = CScrollMgr::GetScrollY();

	Rectangle(hDC, m_tRect.left + fScrollX, m_tRect.top + fScrollY,
		m_tRect.right + fScrollX, m_tRect.bottom + fScrollY);*/
}

void CSkillColBox::Release()
{
}
