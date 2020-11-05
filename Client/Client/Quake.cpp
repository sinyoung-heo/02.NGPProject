#include "stdafx.h"
#include "Quake.h"
#include "MonSkillColBox.h"

static int iTime = 0;

CQuake::CQuake()
{
}

CQuake::~CQuake()
{
	Release();
}

void CQuake::Initialize()
{

	m_tInfo.fCX = 256.f;
	m_tInfo.fCY = 256.f;

	m_tColInfo.fCX = 100.f;
	m_tColInfo.fCY = 100.f;

	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 6;
	m_tFrame.dwFrameSpd = 200;
	m_tFrame.dwOldTime = GetTickCount();

	bIsCreate = false;
}

void CQuake::LateInit()
{
	CObj::UpdateColXYPos();

	CSoundMgr::GetInstance()->PlaySound(L"BossAttack.mp3", CSoundMgr::BOSS_SKILL);

}

int CQuake::Update()
{
	CObj::LateInit();
	if (m_bIsDead)
		return DEAD_OBJ;

	CObj::UpdateColXYPos();

	iTime += 5;

	/*충돌박스 생성 횟수*/
	/*if (!bIsCreate)
	{
		CObjMgr::GetInstance()->AddObject(CAbstractFactory<CMonSkillColBox>::CreateColBox(m_tColInfo.fCX, m_tColInfo.fCY, m_tInfo.fX, m_tInfo.fY, 20),
			ObjID::MON_ATT_COLBOX);

		bIsCreate = true;
	}*/

	return NO_EVENT;
}

void CQuake::LateUpdate()
{
	/*스킬 재생시간.*/
	if (m_tFrame.iFrameStart == m_tFrame.iFrameEnd )
	{
		//CObjMgr::GetInstance()->DeleteObj(ObjID::MON_ATT_COLBOX);
		bIsDead();
		iTime = 0;
	}

	CObj::MoveFrame();
}

void CQuake::Render(HDC hDC)
{
	CObj::UpdateRect();
	CObj::UpdateColRect();


	HDC hMemDC = nullptr;
	NULL_CHECK(hMemDC = CBmpMgr::GetInstance()->FindImage(L"Skill_Quake"));

	float fScrollX = CScrollMgr::GetScrollX();
	float fScrollY = CScrollMgr::GetScrollY();


	GdiTransparentBlt(hDC, m_tRect.left + (int)fScrollX, m_tRect.top + (int)fScrollY, (int)m_tInfo.fCX, (int)m_tInfo.fCY,
		hMemDC, m_tFrame.iFrameStart*(int)m_tInfo.fCX, 0, (int)m_tInfo.fCX, (int)m_tInfo.fCY, RGB(255, 0, 255));
}

void CQuake::Release()
{
}
