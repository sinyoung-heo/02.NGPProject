#include "stdafx.h"
#include "Jelly.h"
#include "MonSkillColBox.h"

#define HP_LEN 50.f

DWORD dwAttackTime = GetTickCount();

CJelly::CJelly()
	:m_pTarget(nullptr), m_bIsHit(false),  m_eState(END_STATE)
{
}


CJelly::~CJelly()
{
	Release();
}

void CJelly::Initialize()
{
	CBmpMgr::GetInstance()->InsertBmp(L"MonsterJelly_LD", L"../Image/Monster/MonsterJelly_LD.bmp");
	CBmpMgr::GetInstance()->InsertBmp(L"MonsterJelly_LU", L"../Image/Monster/MonsterJelly_LU.bmp");
	CBmpMgr::GetInstance()->InsertBmp(L"MonsterJelly_RD", L"../Image/Monster/MonsterJelly_RD.bmp");
	CBmpMgr::GetInstance()->InsertBmp(L"MonsterJelly_RU", L"../Image/Monster/MonsterJelly_RU.bmp");

	srand(unsigned(time(nullptr)));

	/*애니메이션 변수 초기화*/
	m_pFrameKey = L"MonsterJelly_LD";
	m_eCurStance = WALK;
	m_ePreStance = m_eCurStance;

	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 4;
	m_tFrame.iScene = 2;
	m_tFrame.dwOldTime = GetTickCount();
	m_tFrame.dwFrameSpd = 200;

	/*Monster Info*/
	m_tInfo.szName = L"젤리삐";
	m_tInfo.iMaxHp = 50000;
	m_tInfo.iHp = m_tInfo.iMaxHp;
	m_tInfo.iExp = 50;
	m_tInfo.iAtt = 5;

	m_fSpeed = 1.5f;

	m_eState = REST;

	/*이미지 박스*/
	m_tInfo.fCX = 256.f;
	m_tInfo.fCY = 256.f;

	/*충돌 박스*/
	m_tColInfo.fCX = 30.f;
	m_tColInfo.fCY = 30.f;

	/* 공격 방향 & 거리 */
	m_fAttLen = 50.f;

	m_pTarget = CObjMgr::GetInstance()->GetPlayer();
}

void CJelly::LateInit()
{
	CObj::UpdateColXYPos();
}

int CJelly::Update()
{
	CObj::LateInit();
	if (m_bIsDead)
	{
		m_pTarget->SetExp(m_tInfo.iExp);

		return DEAD_OBJ;
	}

	if (m_bIsHit)
	{	/*피격상태이면 피격 모션 실행*/
		m_eCurStance = HIT;

		if (m_tFrame.iFrameStart == m_tFrame.iFrameEnd)
		{	/*애니메이션 끝나면 피격 상태 해제*/
			m_bIsHit = false;
		}
	}
	else
	{	/*피격 상태가 아닐 시에 실행*/
		switch (m_eState)
		{
		case REST:
			
			break;

		case ANGRY:
			ChangeImageKey();
			m_eCurStance = RUN;

			/*Target 추적.*/
			m_fAngle = CMathMgr::CalcRadian(m_pTarget, this);

			m_tInfo.fX += cosf(m_fAngle)*m_fSpeed;
			m_tInfo.fY -= sinf(m_fAngle)*m_fSpeed;
			CObj::UpdateColXYPos();
			
			m_fDist = CMathMgr::CalcDistance(m_pTarget, this);
			if (m_fDist < 60.f)	//일정 거리 가까워지면  RAGE상태로 전환.
				m_eState = RAGE;

			break;

		case RAGE:
			ChangeImageKey();
			m_eCurStance = ATTACK;

			m_fAngle = CMathMgr::CalcRadian(m_pTarget, this);

			m_fAttackX = m_tInfo.fX + cosf(m_fAngle) * m_fAttLen;
			m_fAttackY = m_tInfo.fY - sinf(m_fAngle) * m_fAttLen;

			/*공격 오브젝트 생성*/
			if (m_tFrame.iFrameStart == m_tFrame.iFrameEnd)
			{
				if (dwAttackTime + 150 <= GetTickCount())
				{
					CObjMgr::GetInstance()->AddObject(CAbstractFactory<CMonSkillColBox>::CreateColBox(50.f, 50.f, m_fAttackX, m_fAttackY, 30),
						ObjID::MON_ATT_COLBOX);

					dwAttackTime = GetTickCount();
				}
			}


			m_fDist = CMathMgr::CalcDistance(m_pTarget, this);
			if (m_fDist > 85.f)	//일정 거리 멀어지면  ANGRY상태로 전환.
				m_eState = ANGRY;

			break;
		}
	}



	return NO_EVENT;
}

void CJelly::LateUpdate()
{
	CObj::UpdateColRect();
	CObj::MoveFrame();
	SceneChange();
}

void CJelly::Render(HDC hDC)
{
	CObj::UpdateColXYPos();
	CObj::UpdateRect();
	CObj::UpdateColRect();

	HDC hMemDC = nullptr;
	NULL_CHECK(hMemDC = CBmpMgr::GetInstance()->FindImage(m_pFrameKey.c_str()));

	float fScrollX = CScrollMgr::GetScrollX();
	float fScrollY = CScrollMgr::GetScrollY();

	/*Jelly 출력*/
	GdiTransparentBlt(hDC, m_tRect.left + (int)fScrollX, m_tRect.top + (int)fScrollY, (int)m_tInfo.fCX, (int)m_tInfo.fCY,
		hMemDC, m_tFrame.iFrameStart * (int)m_tInfo.fCX, m_tFrame.iScene * (int)m_tInfo.fCY,
		(int)m_tInfo.fCX, (int)m_tInfo.fCY, RGB(255, 0, 255));

	/* HpBar */
	NULL_CHECK(hMemDC = CBmpMgr::GetInstance()->FindImage(L"Small_HpBarEmpty"));
	BitBlt(hDC, m_tColRect.left + (int)fScrollX, m_tColRect.top - 10 + (int)fScrollY, 50, 3, hMemDC, 0, 0, SRCCOPY);

	NULL_CHECK(hMemDC = CBmpMgr::GetInstance()->FindImage(L"Small_HpBar"));
	GdiTransparentBlt(hDC, m_tColRect.left + (int)fScrollX, m_tColRect.top - 10 + (int)fScrollY,
		(int)(m_tInfo.iHp * HP_LEN) / m_tInfo.iMaxHp, 3, hMemDC, 0, 0, 50, 3, RGB(0, 0, 0));

}

void CJelly::Release()
{

}

void CJelly::SceneChange()
{
	if (m_eCurStance != m_ePreStance)
	{
		switch (m_eCurStance)
		{
		case ATTACK:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 11;
			m_tFrame.iScene = 0;
			m_tFrame.dwOldTime = GetTickCount();
			m_tFrame.dwFrameSpd = 100;
			break;
		case HIT:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 3;
			m_tFrame.iScene = 1;
			m_tFrame.dwOldTime = GetTickCount();
			m_tFrame.dwFrameSpd = 100;
			break;
		case RUN:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 7;
			m_tFrame.iScene = 3;
			m_tFrame.dwOldTime = GetTickCount();
			m_tFrame.dwFrameSpd = 100;
			break;
		case WALK:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 4;
			m_tFrame.iScene = 2;
			m_tFrame.dwOldTime = GetTickCount();
			m_tFrame.dwFrameSpd = 100;
			break;
		}

		m_ePreStance = m_eCurStance;
	}
}

void CJelly::ChangeImageKey()
{
	m_fAngle = CMathMgr::CalcDegree(m_pTarget, this);

	/*몬스터 방향 전환.*/
	if (m_fAngle >= 0 && m_fAngle < 90)
		m_pFrameKey = L"MonsterJelly_RU";
	if (m_fAngle >= 90 && m_fAngle < 180)
		m_pFrameKey = L"MonsterJelly_LU";
	if (m_fAngle >= -90 && m_fAngle < 0)
		m_pFrameKey = L"MonsterJelly_RD";
	if (m_fAngle >= -180 && m_fAngle < -90)
		m_pFrameKey = L"MonsterJelly_LD";
}
