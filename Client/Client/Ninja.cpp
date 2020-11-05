#include "stdafx.h"
#include "Ninja.h"
#include "MonSkillColBox.h"

#define HP_LEN 159

/*보스 위치 바꿔주는 패턴 시간.*/
DWORD dwChangeLocateTime = GetTickCount();
DWORD dwAttackTime3 = GetTickCount();

CNinja::CNinja()
{
}

CNinja::~CNinja()
{
	Release();
}

void CNinja::Initialize()
{
	CBmpMgr::GetInstance()->InsertBmp(L"mn_ld", L"../Image/Monster/mn_ld.bmp");
	CBmpMgr::GetInstance()->InsertBmp(L"mn_lu", L"../Image/Monster/mn_lu.bmp");
	CBmpMgr::GetInstance()->InsertBmp(L"mn_rd", L"../Image/Monster/mn_rd.bmp");
	CBmpMgr::GetInstance()->InsertBmp(L"mn_ru", L"../Image/Monster/mn_ru.bmp");

	CBmpMgr::GetInstance()->InsertBmp(L"BossHp", L"../Image/Monster/BossHp.bmp");

	/*이미지 정보 초기화*/
	m_pFrameKey = L"mn_ld";
	m_eCurStance = IDLE;
	m_ePreStance = m_eCurStance;

	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 9;
	m_tFrame.iScene = 0;
	m_tFrame.dwOldTime = GetTickCount();
	m_tFrame.dwFrameSpd = 100;

	/*Monster Info*/
	m_tInfo.szName = L"닌자";
	m_tInfo.iMaxHp = 200000;
	m_tInfo.iHp = m_tInfo.iMaxHp;
	m_tInfo.iExp = 1000;
	m_tInfo.iAtt = 200;
	m_fSpeed = 2.5f;

	m_eState = REST;

	/*이미지 박스*/
	m_tInfo.fCX = 350.f;
	m_tInfo.fCY = 300.f;

	/*충돌 박스*/
	m_tColInfo.fCX = 50.f;
	m_tColInfo.fCY = 50.f;

	m_bIsHit = false;

	/*공격 사거리*/
	m_fAttLen = 60.f;

	/*타겟 설정*/
	m_pTarget = CObjMgr::GetInstance()->GetPlayer();
}

void CNinja::LateInit()
{
	CObj::UpdateColXYPos();
}

int CNinja::Update()
{
	CObj::LateInit();
	if (m_bIsDead)
	{
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
			m_fDist = CMathMgr::CalcDistance(m_pTarget, this);
			/*일정 범위 안에 있으면 추격 상태*/
			if (m_fDist < 300.f)
				m_eState = ANGRY;

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
			if (m_tInfo.iHp > m_tInfo.iMaxHp * 0.5f)
			{
				ChangeImageKey();
				m_eCurStance = ATTACK;

				m_fAngle = CMathMgr::CalcRadian(m_pTarget, this);

				m_fAttackX = m_tInfo.fX + cosf(m_fAngle) * m_fAttLen;
				m_fAttackY = m_tInfo.fY - sinf(m_fAngle) * m_fAttLen;

				/*공격 오브젝트 생성*/
				if (m_tFrame.iFrameStart == m_tFrame.iFrameEnd)
				{
					if (dwAttackTime3 + 30 <= GetTickCount())
					{
						CSoundMgr::GetInstance()->PlaySound(L"Ninja.wav", CSoundMgr::ATTACK_EFFECT);


						CObjMgr::GetInstance()->AddObject(CAbstractFactory<CMonSkillColBox>::CreateColBox(50.f, 50.f, m_fAttackX, m_fAttackY, m_tInfo.iAtt),
							ObjID::MON_ATT_COLBOX);

						dwAttackTime3 = GetTickCount();
					}
				}
			}
			else
			{
				if (dwChangeLocateTime + 3000 <= GetTickCount())
				{
					if (m_tInfo.fX < m_pTarget->GetInfo().fX)
						m_tInfo.fX += m_pTarget->GetColInfo().fCX *1.5f;
					else
						m_tInfo.fX -= m_pTarget->GetColInfo().fCX * 1.5f;

					dwChangeLocateTime = GetTickCount();
				}
				m_tFrame.dwFrameSpd = 30;

				ChangeImageKey();
				m_eCurStance = SKILL;

				m_fAngle = CMathMgr::CalcRadian(m_pTarget, this);

				m_fAttackX = m_tInfo.fX + cosf(m_fAngle) * m_fAttLen;
				m_fAttackY = m_tInfo.fY - sinf(m_fAngle) * m_fAttLen;

				/*공격 오브젝트 생성*/
				if (m_tFrame.iFrameStart == m_tFrame.iFrameEnd)
				{
					CSoundMgr::GetInstance()->PlaySound(L"Ninja.wav", CSoundMgr::ATTACK_EFFECT);

					CObjMgr::GetInstance()->AddObject(CAbstractFactory<CMonSkillColBox>::CreateColBox(50.f, 50.f, m_fAttackX, m_fAttackY, (int)(m_tInfo.iAtt*1.3f)),
						ObjID::MON_ATT_COLBOX);
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

void CNinja::LateUpdate()
{
	CObj::UpdateColXYPos();
	CObj::MoveFrame();
	SceneChange();
}

void CNinja::Render(HDC hDC)
{
	CObj::UpdateRect();
	CObj::UpdateColRect();

	HDC hMemDC = nullptr;
	NULL_CHECK(hMemDC = CBmpMgr::GetInstance()->FindImage(m_pFrameKey.c_str()));

	float fScrollX = CScrollMgr::GetScrollX();
	float fScrollY = CScrollMgr::GetScrollY();

	GdiTransparentBlt(hDC, m_tRect.left + (int)fScrollX, m_tRect.top + (int)fScrollY, (int)m_tInfo.fCX, (int)m_tInfo.fCY,
		hMemDC, m_tFrame.iFrameStart*(int)m_tInfo.fCX, m_tFrame.iScene*(int)m_tInfo.fCY,
		(int)m_tInfo.fCX, (int)m_tInfo.fCY, RGB(255, 0, 255));

	/*Hp Bar*/
	NULL_CHECK(hMemDC = CBmpMgr::GetInstance()->FindImage(L"BossHp"));
	BitBlt(hDC, 365, 30, m_tInfo.iHp * HP_LEN / m_tInfo.iMaxHp, 15, hMemDC, 0, 0, SRCCOPY);
}

void CNinja::Release()
{

}

void CNinja::SceneChange()
{
	if (m_eCurStance != m_ePreStance)
	{
		switch (m_eCurStance)
		{
		case IDLE:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 9;
			m_tFrame.iScene = 0;
			m_tFrame.dwOldTime = GetTickCount();
			m_tFrame.dwFrameSpd = 100;
			break;
		case RUN:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 4;
			m_tFrame.iScene = 1;
			m_tFrame.dwOldTime = GetTickCount();
			m_tFrame.dwFrameSpd = 100;
			break;
		case SKILL:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 16;
			m_tFrame.iScene = 2;
			m_tFrame.dwOldTime = GetTickCount();
			m_tFrame.dwFrameSpd = 100;
			break;
		case ATTACK:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 15;
			m_tFrame.iScene = 3;
			m_tFrame.dwOldTime = GetTickCount();
			m_tFrame.dwFrameSpd = 100;
			break;
		case DEAD:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 4;
			m_tFrame.iScene = 4;
			m_tFrame.dwOldTime = GetTickCount();
			m_tFrame.dwFrameSpd = 100;
			break;
		}

		m_ePreStance = m_eCurStance;
	}
}

void CNinja::ChangeImageKey()
{
	m_fAngle = CMathMgr::CalcDegree(m_pTarget, this);

	/*몬스터 방향 전환.*/
	if (m_fAngle >= 0 && m_fAngle < 90)
		m_pFrameKey = L"mn_ru";
	if (m_fAngle >= 90 && m_fAngle < 180)
		m_pFrameKey = L"mn_lu";
	if (m_fAngle >= -90 && m_fAngle < 0)
		m_pFrameKey = L"mn_rd";
	if (m_fAngle >= -180 && m_fAngle < -90)
		m_pFrameKey = L"mn_ld";
}
