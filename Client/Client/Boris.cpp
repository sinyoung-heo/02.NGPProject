#include "stdafx.h"
#include "Boris.h"
#include "MonSkillColBox.h"
#include "ElectricBolt.h"
#include "Boom.h"
#include "Quake.h"

#define HP_LEN 159

DWORD dwAttackTime4			= GetTickCount();
DWORD dwElectricBoltTime	= GetTickCount();
DWORD dwBoomTime			= GetTickCount();
DWORD dwQuakeTime			= GetTickCount();
DWORD dwChangeLocate		= GetTickCount();

CBoris::CBoris()
{
}

CBoris::~CBoris()
{
	Release();
}

void CBoris::Initialize()
{
	CBmpMgr::GetInstance()->InsertBmp(L"Boss_Boris_Down", L"../Image/Monster/Boss_Boris_Down.bmp");
	CBmpMgr::GetInstance()->InsertBmp(L"Boss_Boris_Left", L"../Image/Monster/Boss_Boris_Left.bmp");
	CBmpMgr::GetInstance()->InsertBmp(L"Boss_Boris_LeftDown", L"../Image/Monster/Boss_Boris_LeftDown.bmp");
	CBmpMgr::GetInstance()->InsertBmp(L"Boss_Boris_LeftUp", L"../Image/Monster/Boss_Boris_LeftUp.bmp");
	CBmpMgr::GetInstance()->InsertBmp(L"Boss_Boris_Right", L"../Image/Monster/Boss_Boris_Right.bmp");
	CBmpMgr::GetInstance()->InsertBmp(L"Boss_Boris_RightDown", L"../Image/Monster/Boss_Boris_RightDown.bmp");
	CBmpMgr::GetInstance()->InsertBmp(L"Boss_Boris_RightUp", L"../Image/Monster/Boss_Boris_RightUp.bmp");
	CBmpMgr::GetInstance()->InsertBmp(L"Boss_Boris_Up", L"../Image/Monster/Boss_Boris_Up.bmp");

	CBmpMgr::GetInstance()->InsertBmp(L"Boris_Portal_Down", L"../Image/Monster/Boris_Portal_Down.bmp");
	CBmpMgr::GetInstance()->InsertBmp(L"Boris_Portal_Left", L"../Image/Monster/Boris_Portal_Left.bmp");
	CBmpMgr::GetInstance()->InsertBmp(L"Boris_Portal_LeftDown", L"../Image/Monster/Boris_Portal_LeftDown.bmp");
	CBmpMgr::GetInstance()->InsertBmp(L"Boris_Portal_LeftUp", L"../Image/Monster/Boris_Portal_LeftUp.bmp");
	CBmpMgr::GetInstance()->InsertBmp(L"Boris_Portal_Right", L"../Image/Monster/Boris_Portal_Right.bmp");
	CBmpMgr::GetInstance()->InsertBmp(L"Boris_Portal_RightDown", L"../Image/Monster/Boris_Portal_RightDown.bmp");
	CBmpMgr::GetInstance()->InsertBmp(L"Boris_Portal_RightUp", L"../Image/Monster/Boris_Portal_RightUp.bmp");
	CBmpMgr::GetInstance()->InsertBmp(L"Boris_Portal_Up", L"../Image/Monster/Boris_Portal_Up.bmp");

	CBmpMgr::GetInstance()->InsertBmp(L"Boris_Summon", L"../Image/Monster/Boris_Summon.bmp");

	CBmpMgr::GetInstance()->InsertBmp(L"BossHp", L"../Image/Monster/BossHp.bmp");

	m_pFrameKey = L"Boss_Boris_Left";

	m_eCurStance = IDLE;
	m_ePreStance = m_eCurStance;

	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 3;
	m_tFrame.iScene = 12;
	m_tFrame.dwOldTime = GetTickCount();
	m_tFrame.dwFrameSpd = 150;

	m_bIsPortal = false;
	m_potalsound = false;

	/*Monster Info*/
	m_tInfo.szName = L"보리스";

	m_tInfo.iMaxHp = 300000;
	m_tInfo.iHp = m_tInfo.iMaxHp;
	m_tInfo.iExp = 5000;
	m_tInfo.iAtt = 150;
	m_fSpeed = 2.5f;

	m_eState = REST;
	
	/*이미지 박스*/
	m_tInfo.fCX = 220.f;
	m_tInfo.fCY = 220.f;

	/*충돌 박스*/
	m_tColInfo.fCX = 50.f;
	m_tColInfo.fCY = 50.f;

	m_bIsHit = false;

	/*공격 사거리*/
	m_fAttLen = 60.f;

	/*타겟 설정*/
	m_pTarget = CObjMgr::GetInstance()->GetPlayer();

}

void CBoris::LateInit()
{
	CObj::UpdateColXYPos();
}

int CBoris::Update()
{
	CObj::LateInit();
	if (m_bIsDead)
	{
		m_pFrameKey = L"Boss_Boris_Down";
		/*사망상태이면 사망 모션 실행 후 삭제*/
		m_eCurStance = DEAD;
		m_eState = END_STATE;
		m_tFrame.dwFrameSpd = 400;

		if (m_tFrame.iFrameStart == m_tFrame.iFrameEnd)
		{
			m_pTarget->SetExp(m_tInfo.iExp);

			return DEAD_OBJ;
		}
	}

	if (m_bIsHit)
	{	/*피격상태이면 피격 모션 실행*/
		m_eCurStance = IDLE;

		if (m_tFrame.iFrameStart == m_tFrame.iFrameEnd)
		{	/*애니메이션 끝나면 피격 상태 해제*/
			m_bIsHit = false;
		}
	}
	else
	{
		switch (m_eState)
		{
		case REST:
			m_eCurStance = IDLE;

			m_fDist = CMathMgr::CalcDistance(m_pTarget, this);
			/*일정 범위 안에 있으면 추격 상태*/
			if (m_fDist < 350.f)
				m_eState = ANGRY;

			break;

		case ANGRY:
			ChangeImageKey();
			m_eCurStance = RUN;
			//m_bIsPortal = false;

			/*3초간격 electricbolt 생성*/
			if (dwElectricBoltTime + 2500 <= GetTickCount())
			{
				CObjMgr::GetInstance()->AddObject(CAbstractFactory<CElectricBolt>::CreateObj(m_pTarget->GetColInfo().fX, m_pTarget->GetColInfo().fY),
					ObjID::SKILL);

				dwElectricBoltTime = GetTickCount();
			}

			/*Target 추적.*/
			m_fAngle = CMathMgr::CalcRadian(m_pTarget, this);

			m_tInfo.fX += cosf(m_fAngle)*m_fSpeed;
			m_tInfo.fY -= sinf(m_fAngle)*m_fSpeed;
			CObj::UpdateColXYPos();


			m_fDist = CMathMgr::CalcDistance(m_pTarget, this);
			if (m_fDist < 50.f)	//일정 거리 가까워지면  RAGE상태로 전환.
				m_eState = RAGE;

			break;

		case RAGE:
			ChangeImageKey();
			m_eCurStance = ATTACK;

			/*3초간격 electricbolt 생성*/
			if (dwElectricBoltTime + 3000 <= GetTickCount())
			{
				CObjMgr::GetInstance()->AddObject(CAbstractFactory<CElectricBolt>::CreateObj(m_pTarget->GetColInfo().fX, m_pTarget->GetColInfo().fY),
					ObjID::SKILL);

				dwElectricBoltTime = GetTickCount();
			}
			
			/*Hp 70%이하로 떨어질 시 스킬 패턴 발동*/
			if (m_tInfo.iHp <= m_tInfo.iMaxHp*0.7f)
			{
				if (dwBoomTime + 3000 <= GetTickCount())
				{
					CObjMgr::GetInstance()->AddObject(CAbstractFactory<CBoom>::CreateObj(m_pTarget->GetColInfo().fX, m_pTarget->GetColInfo().fY),
						ObjID::SKILL);

					dwBoomTime = GetTickCount();
				}
			}

			/*Hp 40%이하로 떨어질 시 CRAZY상태로 전환*/
			if (m_tInfo.iHp <= m_tInfo.iMaxHp*0.4f)
			{
				m_eState = CRAZY;
				m_bIsPortal = true;
			}

			/*기본 공격*/
			m_fAngle = CMathMgr::CalcRadian(m_pTarget, this);

			m_fAttackX = m_tInfo.fX + cosf(m_fAngle) * m_fAttLen;
			m_fAttackY = m_tInfo.fY - sinf(m_fAngle) * m_fAttLen;

			if (m_tFrame.iFrameStart == m_tFrame.iFrameEnd)
			{
				if (dwAttackTime4 + 50 <= GetTickCount())
				{
					CSoundMgr::GetInstance()->PlaySound(L"Hit.wav", CSoundMgr::BOSS_ATTACK);


					CObjMgr::GetInstance()->AddObject(CAbstractFactory<CMonSkillColBox>::CreateColBox(50.f, 50.f, m_fAttackX, m_fAttackY, m_tInfo.iAtt),
						ObjID::MON_ATT_COLBOX);

					dwAttackTime4 = GetTickCount();
				}
			}


			m_fDist = CMathMgr::CalcDistance(m_pTarget, this);
			if (m_fDist > 70.f)	//일정 거리 멀어지면  ANGRY상태로 전환.
				m_eState = ANGRY;

			break;

		case CRAZY:
			ChangeImageKey();
			m_eCurStance = PORTAL_ATTACK;


			/*2초간격 electricbolt 생성*/
			if (dwElectricBoltTime + 7000 <= GetTickCount())
			{
				CObjMgr::GetInstance()->AddObject(CAbstractFactory<CElectricBolt>::CreateObj(m_pTarget->GetColInfo().fX, m_pTarget->GetColInfo().fY),
					ObjID::SKILL);

				dwElectricBoltTime = GetTickCount();
			}
			/*Hp 70%이하로 떨어질 시 스킬 Boom 패턴 발동*/
			if (m_tInfo.iHp <= m_tInfo.iMaxHp*0.7f)
			{
				if (dwBoomTime + 3000 <= GetTickCount())
				{
					CObjMgr::GetInstance()->AddObject(CAbstractFactory<CBoom>::CreateObj(m_pTarget->GetColInfo().fX, m_pTarget->GetColInfo().fY),
						ObjID::SKILL);

					dwBoomTime = GetTickCount();
				}
			}

			if (m_tFrame.iFrameStart == m_tFrame.iFrameEnd)
			{	
				/*1.5초마다 위치 텔레포트*/
				if (dwChangeLocate + 1500 <= GetTickCount())
				{
					float fScrollX = CScrollMgr::GetScrollX();

					if (m_tInfo.fX < m_pTarget->GetInfo().fX)
						m_tInfo.fX = m_pTarget->GetColInfo().fX + 30.f;
					else
						m_tInfo.fX = m_pTarget->GetColInfo().fX - 30.f;

					if (m_tInfo.fY < m_pTarget->GetInfo().fY)
					{
						m_tInfo.fX = m_pTarget->GetColInfo().fX - 30.f;
						m_tInfo.fY = m_pTarget->GetColInfo().fY;
					}
					else
					{
						m_tInfo.fX = m_pTarget->GetColInfo().fX + 30.f;
						m_tInfo.fY = m_pTarget->GetColInfo().fY;
					}

					ChangeImageKey();

					dwChangeLocate = GetTickCount();
				}

				/*기본 공격*/
				m_fAngle = CMathMgr::CalcRadian(m_pTarget, this);

				m_fAttackX = m_tInfo.fX + cosf(m_fAngle) * m_fAttLen;
				m_fAttackY = m_tInfo.fY - sinf(m_fAngle) * m_fAttLen;

				if (dwAttackTime4 + 50 <= GetTickCount())
				{
					if(!m_potalsound)
						CSoundMgr::GetInstance()->PlaySound(L"BossPotalSkill.wav", CSoundMgr::BOSS_ATTACK);

					m_potalsound = true;

					CObjMgr::GetInstance()->AddObject(CAbstractFactory<CMonSkillColBox>::CreateColBox(50.f, 50.f, m_fAttackX, m_fAttackY, m_tInfo.iAtt * 2),
						ObjID::MON_ATT_COLBOX);

					dwAttackTime4 = GetTickCount();
				}

				m_potalsound = false;
			}


			if (dwQuakeTime + 1500 <= GetTickCount())
			{
				CObjMgr::GetInstance()->AddObject(CAbstractFactory<CQuake>::CreateObj(m_tInfo.fX, m_tInfo.fY),
					ObjID::SKILL);

				dwQuakeTime = GetTickCount();
			}

			//m_fDist = CMathMgr::CalcDistance(m_pTarget, this);
			//if (m_fDist > 80.f)	//일정 거리 멀어지면  ANGRY상태로 전환.
			//	m_eState = ANGRY;

			break;
		}
	}

	return NO_EVENT;
}

void CBoris::LateUpdate()
{
	CObj::UpdateColXYPos();
	CObj::MoveFrame();

	SceneChange();
}

void CBoris::Render(HDC hDC)
{
	CObj::UpdateRect();
	CObj::UpdateColRect();

	HDC hMemDC = nullptr;
	NULL_CHECK(hMemDC = CBmpMgr::GetInstance()->FindImage(m_pFrameKey.c_str()));

	float fScrollX = CScrollMgr::GetScrollX();
	float fScrollY = CScrollMgr::GetScrollY();

	GdiTransparentBlt(hDC, m_tRect.left + (int)fScrollX, m_tRect.top + (int)fScrollY, (int)m_tInfo.fCX, (int)m_tInfo.fCY,
		hMemDC, m_tFrame.iFrameStart * (int)m_tInfo.fCX, m_tFrame.iScene* (int)m_tInfo.fCY,
		(int)m_tInfo.fCX, (int)m_tInfo.fCY, RGB(64, 170, 226));

	/*Hp Bar*/
	NULL_CHECK(hMemDC = CBmpMgr::GetInstance()->FindImage(L"BossHp"));
	BitBlt(hDC, 365, 30, m_tInfo.iHp * HP_LEN / m_tInfo.iMaxHp, 15, hMemDC, 0, 0, SRCCOPY);
}

void CBoris::Release()
{

}

void CBoris::SceneChange()
{
	if (m_eCurStance != m_ePreStance)
	{
		switch (m_eCurStance)
		{
		case IDLE:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 3;
			m_tFrame.iScene = 0;
			m_tFrame.dwOldTime = GetTickCount();
			m_tFrame.dwFrameSpd = 200;
			break;
		case RUN:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 9;
			m_tFrame.iScene = 1;
			m_tFrame.dwOldTime = GetTickCount();
			m_tFrame.dwFrameSpd = 150;
			break;

		case ATTACK:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 6;
			m_tFrame.iScene = 2;
			m_tFrame.dwOldTime = GetTickCount();
			m_tFrame.dwFrameSpd = 130;
			break;

		case PORTAL_ATTACK:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 5;
			m_tFrame.iScene = 0;
			m_tFrame.dwOldTime = GetTickCount();
			m_tFrame.dwFrameSpd = 200;
			break;

		case DEAD:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 2;
			m_tFrame.iScene = 3;
			m_tFrame.dwOldTime = GetTickCount();
			m_tFrame.dwFrameSpd = 800;
			break;
		}

		m_ePreStance = m_eCurStance;
	}
}

void CBoris::ChangeImageKey()
{
	m_fAngle = CMathMgr::CalcDegree(m_pTarget, this);

	/*몬스터 방향 전환.*/
	if (m_fAngle > -30.f && m_fAngle <= 30.f)
	{
		if (m_eCurStance == PORTAL_ATTACK && m_bIsPortal == true)	/*공격 모션 변화*/
			m_pFrameKey = L"Boris_Portal_Right";
		else
			m_pFrameKey = L"Boss_Boris_Right";
	}

	if (m_fAngle > 30 && m_fAngle <= 60.f)
	{
		if (m_eCurStance == PORTAL_ATTACK && m_bIsPortal == true)	/*공격 모션 변화*/
			m_pFrameKey = L"Boris_Portal_RightUp";
		else
			m_pFrameKey = L"Boss_Boris_RightUp";
	}

	if (m_fAngle > 60 && m_fAngle <= 120.f)
	{
		if (m_eCurStance == PORTAL_ATTACK && m_bIsPortal == true)	/*공격 모션 변화*/
			m_pFrameKey = L"Boris_Portal_Up";
		else
			m_pFrameKey = L"Boss_Boris_Up";
	}

	if (m_fAngle > 120 && m_fAngle <= 150.f)
	{
		if (m_eCurStance == PORTAL_ATTACK && m_bIsPortal == true)	/*공격 모션 변화*/
			m_pFrameKey = L"Boris_Portal_LeftUp";
		else
			m_pFrameKey = L"Boss_Boris_LeftUp";
	}

	if (m_fAngle > 150 && m_fAngle <= 180 || m_fAngle < -150.f && m_fAngle >= -180.f)
	{
		if (m_eCurStance == PORTAL_ATTACK && m_bIsPortal == true)	/*공격 모션 변화*/
			m_pFrameKey = L"Boris_Portal_Left";
		else
			m_pFrameKey = L"Boss_Boris_Left";
	}

	if (m_fAngle > -150 && m_fAngle <= -120.f)
	{
		if (m_eCurStance == PORTAL_ATTACK && m_bIsPortal == true)	/*공격 모션 변화*/
			m_pFrameKey = L"Boris_Portal_LeftDown";
		else
			m_pFrameKey = L"Boss_Boris_LeftDown";
	}

	if (m_fAngle > -120 && m_fAngle <= -60.f)
	{
		if (m_eCurStance == PORTAL_ATTACK && m_bIsPortal == true)	/*공격 모션 변화*/
			m_pFrameKey = L"Boris_Portal_Down";
		else
			m_pFrameKey = L"Boss_Boris_Down";
	}

	if (m_fAngle > -60.f && m_fAngle <= -30.f)
	{
		if (m_eCurStance == PORTAL_ATTACK && m_bIsPortal == true)	/*공격 모션 변화*/
			m_pFrameKey = L"Boris_Portal_RightDown";
		else
			m_pFrameKey = L"Boss_Boris_RightDown";
	}

}
