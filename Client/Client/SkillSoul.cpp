#include "stdafx.h"
#include "SkillSoul.h"
#include "SkillColBox.h"

static int iTime = 0;

CSkillSoul::CSkillSoul()
	:m_pPlayer(nullptr)
{
}

CSkillSoul::~CSkillSoul()
{
	Release();
}

void CSkillSoul::Initialize()
{

	m_tInfo.fCX = 500.f;
	m_tInfo.fCY = 400.f;

	CObj::UpdateColXYPos();		//이미지박스 좌표 & 충돌박스 좌표 같게.
	m_tColInfo.fCX = m_tInfo.fCX * 0.5f;
	m_tColInfo.fCY = m_tInfo.fCY * 0.5f;

	m_fSpeed = 7.f;

	m_tFrame.iFrameStart = 0;

	m_pPlayer = CObjMgr::GetInstance()->GetPlayer();
}

void CSkillSoul::LateInit()
{
	CSoundMgr::GetInstance()->PlaySound(L"SKILL_S.wav", CSoundMgr::PLAYER_SKILL);


	switch ((int)m_fAngle)
	{
	case 270:
		m_tFrame.iFrameStart = 0;
		break;
	case 90:
		m_tFrame.iFrameStart = 1;
		break;
	case 240:
		m_tFrame.iFrameStart = 2;
		break;
	case 180:
		m_tFrame.iFrameStart = 3;
		break;
	case 125:
		m_tFrame.iFrameStart = 4;
		break;
	case 300:
		m_tFrame.iFrameStart = 5;
		break;
	case 360:
		m_tFrame.iFrameStart = 6;
		break;
	case 55:
		m_tFrame.iFrameStart = 7;
		break;
	}
}

int CSkillSoul::Update()
{
	CObj::LateInit();

	if (m_bIsDead)
		return DEAD_OBJ;


	m_tInfo.fX += cosf(m_fAngle * PI / 180.f) * m_fSpeed;
	m_tInfo.fY += sinf(m_fAngle * PI / 180.f) * m_fSpeed;

	CObj::UpdateColXYPos();	//충돌박스 x,y좌표 = 이미지 x,y 좌표

	iTime += 10;

	if (iTime == 100)
	{
		CObjMgr::GetInstance()->AddObject(CAbstractFactory<CSkillColBox>::CreateColBox(m_tInfo.fCX, m_tInfo.fCY/2, m_tInfo.fX, m_tInfo.fY, (int)(m_pPlayer->GetInfo().iAtt*1.2f)),
			ObjID::SKILL_COLBOX);
	}

	return NO_EVENT;
}

void CSkillSoul::LateUpdate()
{
	/*스킬 재생시간.*/
	if (iTime > 500)
	{
		CObjMgr::GetInstance()->DeleteObj(ObjID::SKILL_COLBOX);
		bIsDead();
		iTime = 0;
	}
}

void CSkillSoul::Render(HDC hDC)
{
	CObj::UpdateRect();
	CObj::UpdateColRect();

	HDC hMemDC;
	NULL_CHECK(hMemDC = CBmpMgr::GetInstance()->FindImage(L"Skill_Soul"));

	float fScrollX = CScrollMgr::GetScrollX();
	float fScrollY = CScrollMgr::GetScrollY();

	GdiTransparentBlt(hDC, m_tRect.left + (int)fScrollX, m_tRect.top + (int)fScrollY, (int)m_tInfo.fCX, (int)m_tInfo.fCY,
		hMemDC, m_tFrame.iFrameStart*(int)m_tInfo.fCX, 0, (int)m_tInfo.fCX, (int)m_tInfo.fCY, RGB(255, 255, 255));
}

void CSkillSoul::Release()
{
}
