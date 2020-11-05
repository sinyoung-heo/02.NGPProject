#include "stdafx.h"
#include "Boom.h"
#include "MonSkillColBox.h"

static int iTime = 0;

CBoom::CBoom()
{
}

CBoom::~CBoom()
{
	Release();
}

void CBoom::Initialize()
{

	m_tInfo.fCX = 128.f;
	m_tInfo.fCY = 128.f;

	m_tColInfo.fCX = 128.f;
	m_tColInfo.fCY = 128.f;

	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 10;
	m_tFrame.dwFrameSpd = 150;
	m_tFrame.dwOldTime = GetTickCount();

	bIsCreate = false;
}

void CBoom::LateInit()
{
	CObj::UpdateColXYPos();

	CSoundMgr::GetInstance()->PlaySound(L"Boom.wav", CSoundMgr::BOSS_SKILL);

}

int CBoom::Update()
{
	CObj::LateInit();
	if (m_bIsDead)
		return DEAD_OBJ;

	CObj::UpdateColXYPos();

	iTime += 5;

	/*충돌박스 생성 횟수*/
	if (!bIsCreate)
	{
		CObjMgr::GetInstance()->AddObject(CAbstractFactory<CMonSkillColBox>::CreateColBox(m_tColInfo.fCX, m_tColInfo.fCY, m_tInfo.fX, m_tInfo.fY, 130),
			ObjID::MON_ATT_COLBOX);

		bIsCreate = true;
	}

	return NO_EVENT;
}

void CBoom::LateUpdate()
{
	/*스킬 재생시간.*/
	if (m_tFrame.iFrameStart == m_tFrame.iFrameEnd)
	{

		CObjMgr::GetInstance()->DeleteObj(ObjID::MON_ATT_COLBOX);
		bIsDead();
		iTime = 0;
	}

	CObj::MoveFrame();
}

void CBoom::Render(HDC hDC)
{
	CObj::UpdateRect();
	CObj::UpdateColRect();

	HDC hMemDC = nullptr;
	NULL_CHECK(hMemDC = CBmpMgr::GetInstance()->FindImage(L"Skill_Boom"));

	float fScrollX = CScrollMgr::GetScrollX();
	float fScrollY = CScrollMgr::GetScrollY();

	GdiTransparentBlt(hDC, m_tRect.left + (int)fScrollX, m_tRect.top + (int)fScrollY, (int)m_tInfo.fCX, (int)m_tInfo.fCY,
		hMemDC, m_tFrame.iFrameStart*(int)m_tInfo.fCX, 0, (int)m_tInfo.fCX, (int)m_tInfo.fCY, RGB(255, 0, 255));
}

void CBoom::Release()
{
}
