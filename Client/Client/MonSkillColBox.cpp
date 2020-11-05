#include "stdafx.h"
#include "MonSkillColBox.h"

static int iTime = 0;

CMonSkillColBox::CMonSkillColBox()
{
}

CMonSkillColBox::~CMonSkillColBox()
{
	Release();
}

void CMonSkillColBox::Initialize()
{
	CObj::UpdateRect();
}

void CMonSkillColBox::LateInit()
{
	CSoundMgr::GetInstance()->PlaySound(L"Hit.wav", CSoundMgr::ATTACK_EFFECT);

}

int CMonSkillColBox::Update()
{
	CObj::LateInit();
	if (m_bIsDead)
		return DEAD_OBJ;



	iTime += 5;

	return NO_EVENT;
}

void CMonSkillColBox::LateUpdate()
{
	/*스킬 재생시간.*/
	if (iTime > 40)
	{
		bIsDead();
		iTime = 0;
	}
}

void CMonSkillColBox::Render(HDC hDC)
{
	CObj::UpdateRect();

	/*float fScrollX = CScrollMgr::GetScrollX();
	float fScrollY = CScrollMgr::GetScrollY();

	Rectangle(hDC, m_tRect.left + fScrollX, m_tRect.top + fScrollY,
		m_tRect.right + fScrollX, m_tRect.bottom + fScrollY);*/
}

void CMonSkillColBox::Release()
{
}
