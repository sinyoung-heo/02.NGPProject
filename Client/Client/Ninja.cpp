#include "stdafx.h"
#include "Ninja.h"
#include "MonSkillColBox.h"

#define HP_LEN 159

/*���� ��ġ �ٲ��ִ� ���� �ð�.*/
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

	/*�̹��� ���� �ʱ�ȭ*/
	m_pFrameKey = L"mn_ld";
	m_eCurStance = IDLE;
	m_ePreStance = m_eCurStance;

	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 9;
	m_tFrame.iScene = 0;
	m_tFrame.dwOldTime = GetTickCount();
	m_tFrame.dwFrameSpd = 100;

	/*Monster Info*/
	m_tInfo.szName = L"����";
	m_tInfo.iMaxHp = 200000;
	m_tInfo.iHp = m_tInfo.iMaxHp;
	m_tInfo.iExp = 1000;
	m_tInfo.iAtt = 200;
	m_fSpeed = 2.5f;

	m_eState = REST;

	/*�̹��� �ڽ�*/
	m_tInfo.fCX = 350.f;
	m_tInfo.fCY = 300.f;

	/*�浹 �ڽ�*/
	m_tColInfo.fCX = 50.f;
	m_tColInfo.fCY = 50.f;

	m_bIsHit = false;

	/*���� ��Ÿ�*/
	m_fAttLen = 60.f;

	/*Ÿ�� ����*/
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
		/*��������̸� ��� ��� ���� �� ����*/
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
	{	/*�ǰݻ����̸� �ǰ� ��� ����*/
		m_eCurStance = IDLE;

		if (m_tFrame.iFrameStart == m_tFrame.iFrameEnd)
		{	/*�ִϸ��̼� ������ �ǰ� ���� ����*/
			m_bIsHit = false;
		}
	}
	else
	{
		switch (m_eState)
		{
		case REST:
			m_fDist = CMathMgr::CalcDistance(m_pTarget, this);
			/*���� ���� �ȿ� ������ �߰� ����*/
			if (m_fDist < 300.f)
				m_eState = ANGRY;

			break;
		case ANGRY:
			ChangeImageKey();
			m_eCurStance = RUN;

			/*Target ����.*/
			m_fAngle = CMathMgr::CalcRadian(m_pTarget, this);

			m_tInfo.fX += cosf(m_fAngle)*m_fSpeed;
			m_tInfo.fY -= sinf(m_fAngle)*m_fSpeed;
			CObj::UpdateColXYPos();

			m_fDist = CMathMgr::CalcDistance(m_pTarget, this);
			if (m_fDist < 60.f)	//���� �Ÿ� ���������  RAGE���·� ��ȯ.
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

				/*���� ������Ʈ ����*/
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

				/*���� ������Ʈ ����*/
				if (m_tFrame.iFrameStart == m_tFrame.iFrameEnd)
				{
					CSoundMgr::GetInstance()->PlaySound(L"Ninja.wav", CSoundMgr::ATTACK_EFFECT);

					CObjMgr::GetInstance()->AddObject(CAbstractFactory<CMonSkillColBox>::CreateColBox(50.f, 50.f, m_fAttackX, m_fAttackY, (int)(m_tInfo.iAtt*1.3f)),
						ObjID::MON_ATT_COLBOX);
				}

			}

			m_fDist = CMathMgr::CalcDistance(m_pTarget, this);
			if (m_fDist > 85.f)	//���� �Ÿ� �־�����  ANGRY���·� ��ȯ.
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

	/*���� ���� ��ȯ.*/
	if (m_fAngle >= 0 && m_fAngle < 90)
		m_pFrameKey = L"mn_ru";
	if (m_fAngle >= 90 && m_fAngle < 180)
		m_pFrameKey = L"mn_lu";
	if (m_fAngle >= -90 && m_fAngle < 0)
		m_pFrameKey = L"mn_rd";
	if (m_fAngle >= -180 && m_fAngle < -90)
		m_pFrameKey = L"mn_ld";
}
