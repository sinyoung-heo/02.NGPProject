#include "stdafx.h"
#include "Cow.h"
#include "MonSkillColBox.h"

#define HP_LEN 50.f

DWORD dwAttackTime2 = GetTickCount();

CCow::CCow()
{

}

CCow::~CCow()
{
	Release();
}

void CCow::Initialize()
{
	/*몬스터 비트맵*/
	CBmpMgr::GetInstance()->InsertBmp(L"MonsterC_LD", L"../Image/Monster/MonsterC_LD.bmp");
	CBmpMgr::GetInstance()->InsertBmp(L"MonsterC_LU", L"../Image/Monster/MonsterC_LU.bmp");
	CBmpMgr::GetInstance()->InsertBmp(L"MonsterC_RD", L"../Image/Monster/MonsterC_RD.bmp");
	CBmpMgr::GetInstance()->InsertBmp(L"MonsterC_RU", L"../Image/Monster/MonsterC_RU.bmp");

	/*이미지 정보 초기화*/
	m_pFrameKey = L"MonsterC_LD";
	m_eCurStance = IDLE;
	m_ePreStance = m_eCurStance;

	m_tFrame.iFrameStart	= 0;
	m_tFrame.iFrameEnd		= 5;
	m_tFrame.iScene			= 2;
	m_tFrame.dwOldTime		= GetTickCount();
	m_tFrame.dwFrameSpd		= 150;

	/*Monster Info*/
	m_tInfo.szName		= L"포운";
	m_tInfo.iMaxHp		= 100000;
	m_tInfo.iHp			= m_tInfo.iMaxHp;
	m_tInfo.iExp		= 150;
	m_tInfo.iAtt		= 10;
	m_fSpeed			= 2.5f;

	m_eState = REST;

	/*이미지 박스*/
	m_tInfo.fCX = 256.f;
	m_tInfo.fCY = 256.f;

	/*충돌 박스*/
	m_tColInfo.fCX = 50.f;
	m_tColInfo.fCY = 50.f;

	m_bIsHit = false;

	/*공격 사거리*/
	m_fAttLen = 50.f;

	/*타겟 설정*/
	// m_pTarget = CObjMgr::GetInstance()->GetPlayer();
}

void CCow::LateInit()
{
	CObj::UpdateColXYPos();
}

int CCow::Update()
{
	CObj::LateInit();

	if (m_bIsDead)
		return DEAD_OBJ;

	if (m_tInfo.iHp <= 0)
	{
		m_bIsDead = true;
	}

	//if (m_bIsDead)
	//{

	//	m_eCurStance = DEAD;
	//	m_eState = END_STATE;

	//	if (m_tFrame.iFrameStart == m_tFrame.iFrameEnd)
	//	{
	//		m_pTarget->SetExp(m_tInfo.iExp);

	//		return DEAD_OBJ;
	//	}
	//}

	//if (m_bIsHit)
	//{
	//	/*사망상태이면 사망 모션 실행 후 삭제*/
	//	m_eCurStance = IDLE;

	//	if (m_tFrame.iFrameStart == m_tFrame.iFrameEnd)
	//	{	/*애니메이션 끝나면 피격 상태 해제*/
	//		m_bIsHit = false;
	//	}
	//}
	//else
	//{
	//	switch (m_eState)
	//	{
	//	case REST:
	//		m_fDist = CMathMgr::CalcDistance(m_pTarget, this);
	//		/*일정 범위 안에 있으면 추격 상태*/
	//		if (m_fDist < 250.f)
	//			m_eState = ANGRY;

	//		break;
	//	case ANGRY:
	//		ChangeImageKey();
	//		m_eCurStance = RUN;

	//		/*Target 추적.*/
	//		m_fAngle = CMathMgr::CalcRadian(m_pTarget, this);

	//		m_tInfo.fX += cosf(m_fAngle)*m_fSpeed;
	//		m_tInfo.fY -= sinf(m_fAngle)*m_fSpeed;
	//		CObj::UpdateColXYPos();

	//		m_fDist = CMathMgr::CalcDistance(m_pTarget, this);
	//		if (m_fDist < 60.f)	//일정 거리 가까워지면  RAGE상태로 전환.
	//			m_eState = RAGE;

	//		break;
	//	case RAGE:
	//		ChangeImageKey();
	//		m_eCurStance = ATTACK;

	//		m_fAngle = CMathMgr::CalcRadian(m_pTarget, this);

	//		m_fAttackX = m_tInfo.fX + cosf(m_fAngle) * m_fAttLen;
	//		m_fAttackY = m_tInfo.fY - sinf(m_fAngle) * m_fAttLen;

	//		/*공격 오브젝트 생성*/
	//		if (m_tFrame.iFrameStart == m_tFrame.iFrameEnd)
	//		{
	//			if (dwAttackTime2 + 100 <= GetTickCount())
	//			{
	//				CSoundMgr::GetInstance()->PlaySound(L"Cow.wav", CSoundMgr::ATTACK_EFFECT);


	//				CObjMgr::GetInstance()->AddObject(CAbstractFactory<CMonSkillColBox>::CreateColBox(50.f, 50.f, m_fAttackX, m_fAttackY, 30),
	//					ObjID::MON_ATT_COLBOX);

	//				dwAttackTime2 = GetTickCount();
	//			}
	//		}

	//		m_fDist = CMathMgr::CalcDistance(m_pTarget, this);
	//		if (m_fDist > 85.f)	//일정 거리 멀어지면  ANGRY상태로 전환.
	//			m_eState = ANGRY;

	//		break;
	//	}
	//}


	return NO_EVENT;
}

void CCow::LateUpdate()
{
	CObj::UpdateColXYPos();

	// CObj::MoveFrame();
	ChangeImageKey();
	SceneChange();
}

void CCow::Render(HDC hDC)
{
	CObj::UpdateRect();
	CObj::UpdateColRect();

	HDC hMemDC = nullptr;
	NULL_CHECK(hMemDC = CBmpMgr::GetInstance()->FindImage(m_pFrameKey.c_str()));

	float fScrollX = CScrollMgr::GetScrollX();
	float fScrollY = CScrollMgr::GetScrollY();

	/*Cow 출력*/
	GdiTransparentBlt(hDC,
					  m_tRect.left + (int)fScrollX, 
					  m_tRect.top + (int)fScrollY, 
					  (int)m_tInfo.fCX,
					  (int)m_tInfo.fCY,
					  hMemDC, 
					  m_tFrame.iFrameStart * (int)m_tInfo.fCX,
					  m_tFrame.iScene * (int)m_tInfo.fCY,
					  (int)m_tInfo.fCX, 
					  (int)m_tInfo.fCY, 
					  RGB(255, 0, 255));

	/*HP bar*/
	NULL_CHECK(hMemDC = CBmpMgr::GetInstance()->FindImage(L"Small_HpBarEmpty"));
	BitBlt(hDC, m_tColRect.left + (int)fScrollX, m_tColRect.top - 40 + (int)fScrollY, 50, 3, hMemDC, 0, 0, SRCCOPY);

	NULL_CHECK(hMemDC = CBmpMgr::GetInstance()->FindImage(L"Small_HpBar"));
	GdiTransparentBlt(hDC, m_tColRect.left + (int)fScrollX, m_tColRect.top - 40 + (int)fScrollY,
		(int)(m_tInfo.iHp * HP_LEN) / m_tInfo.iMaxHp, 3, hMemDC, 0, 0, 50, 3, RGB(0, 0, 0));
}

void CCow::Release()
{

}

void CCow::SceneChange()
{
	if (m_eCurStance != m_ePreStance)
	{
		switch (m_eCurStance)
		{
		case ATTACK:
			// m_tFrame.iFrameStart	= 0;
			// m_tFrame.iFrameEnd		= 9;
			m_tFrame.iScene			= 0;
			// m_tFrame.dwOldTime		= GetTickCount();
			// m_tFrame.dwFrameSpd		= 150;
			break;
		case DEAD:
			// m_tFrame.iFrameStart	= 0;
			// m_tFrame.iFrameEnd		= 10;
			m_tFrame.iScene			= 1;
			// m_tFrame.dwOldTime		= GetTickCount();
			// m_tFrame.dwFrameSpd		= 200;
			break;
		case IDLE:
			// m_tFrame.iFrameStart	= 0;
			// m_tFrame.iFrameEnd		= 5;
			m_tFrame.iScene			= 2;
			// m_tFrame.dwOldTime		= GetTickCount();
			// m_tFrame.dwFrameSpd		= 150;
			break;
		case RUN:
			// m_tFrame.iFrameStart	= 0;
			// m_tFrame.iFrameEnd		= 7;
			m_tFrame.iScene			= 3;
			// m_tFrame.dwOldTime		= GetTickCount();
			// m_tFrame.dwFrameSpd		= 150;
			break;
		}

		m_ePreStance = m_eCurStance;
	}
}

void CCow::ChangeImageKey()
{
	// m_fAngle = CMathMgr::CalcDegree(m_pTarget, this);

	/*몬스터 방향 전환.*/
	if (m_fAngle >= 0 && m_fAngle < 90)
	{
		m_pFrameKey = L"MonsterC_RU";
	}
	if (m_fAngle >= 90 && m_fAngle < 180)
	{
		m_pFrameKey = L"MonsterC_LU";
	}
	if (m_fAngle >= -90 && m_fAngle < 0)
	{
		m_pFrameKey = L"MonsterC_RD";
	}
	if (m_fAngle >= -180 && m_fAngle < -90)
	{
		m_pFrameKey = L"MonsterC_LD";
	}
}
