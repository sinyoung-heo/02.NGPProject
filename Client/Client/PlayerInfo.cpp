#include "stdafx.h"
#include "PlayerInfo.h"
#include "Player.h"

#define EXP_LEN 233.f

DWORD dwUpdateOldPos = GetTickCount();

CPlayerInfo::CPlayerInfo()
	:m_pPlayer(nullptr), m_bIsInfoOn(false), m_fOldXpos(0.f), m_fOldYpos(0.f),
	m_fMoveXpos(0.f), m_fMoveYpos(0.f)
{
	ZeroMemory(&m_TextRect1, sizeof(RECT));
	ZeroMemory(&m_TextRect2, sizeof(RECT));
	ZeroMemory(&m_TextRect3, sizeof(RECT));
}

CPlayerInfo::~CPlayerInfo()
{
	Release();
}

void CPlayerInfo::Initialize()
{
	CBmpMgr::GetInstance()->InsertBmp(L"Player_Stats", L"../Image/Ui/Player_Stats.bmp");
	CBmpMgr::GetInstance()->InsertBmp(L"ExpBar", L"../Image/Ui/ExpBar.bmp");
	CBmpMgr::GetInstance()->InsertBmp(L"Exp", L"../Image/Ui/Exp.bmp");

	m_eLayer = Layer::UI;

	m_tInfo.fCX = 345.f;
	m_tInfo.fCY = 305.f;

	m_Text1.fCX = 122.f;
	m_Text1.fCY = 68.f;
	m_Text1.fX = 723.f;
	m_Text1.fY = 80.f;

	m_Text2.fCX = 60.f;
	m_Text2.fCY = 110.f;
	m_Text2.fX = 530.f;
	m_Text2.fY = 245.f;

	m_Text3.fCX = 125.f;
	m_Text3.fCY = 100.f;
	m_Text3.fX = 723.f;
	m_Text3.fY = 240.f;

	m_Text4.fCX = 233.f;
	m_Text4.fCY = 2.f;
	m_Text4.fX = 603.f;
	m_Text4.fY = 155.f;


	m_pPlayer = CObjMgr::GetInstance()->GetPlayer();
	m_tAbility = static_cast<CPlayer*>(m_pPlayer)->GetAbility();
}

void CPlayerInfo::LateInit()
{

}

int CPlayerInfo::Update()
{
	CObj::LateInit();
	if (m_bIsDead)
		return DEAD_OBJ;

	m_tAbility = static_cast<CPlayer*>(m_pPlayer)->GetAbility();

	/*POINT pt = {};
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);
	cout << pt.x << " " << pt.y << endl;*/

	if (m_bIsInfoOn)
	{
		POINT Curpt = {};
		GetCursorPos(&Curpt);
		ScreenToClient(g_hWnd, &Curpt);


		if (PtInRect(&m_tRect, Curpt))
		{
			if (KEY_PRESSING(VK_LBUTTON))
			{
				/* 주기적으로 old 좌푝 갱신 */
				if (dwUpdateOldPos + 100 <= GetTickCount())
				{
					m_fOldXpos = (float)Curpt.x;
					m_fOldYpos = (float)Curpt.y;

					dwUpdateOldPos = GetTickCount();
				}
				/*클릭 중일 때 x 좌표 - 현재 좌표 */
				float fCurXpos = (float)Curpt.x;
				float fCurYpos = (float)Curpt.y;

				/*현재 마우스 좌표 & 이전 마우스 좌표 차로 도형 이동. */
				m_fMoveXpos = fCurXpos - m_fOldXpos;
				m_fMoveYpos = fCurYpos - m_fOldYpos;

				/*텍스트 박스도같이 이동시켜준다.*/
				m_tInfo.fX += m_fMoveXpos*0.4f;
				m_tInfo.fY += m_fMoveYpos*0.4f;

				m_Text1.fX += m_fMoveXpos*0.4f;
				m_Text1.fY += m_fMoveYpos*0.4f;

				m_Text2.fX += m_fMoveXpos*0.4f;
				m_Text2.fY += m_fMoveYpos*0.4f;

				m_Text3.fX += m_fMoveXpos*0.4f;
				m_Text3.fY += m_fMoveYpos*0.4f;

				m_Text4.fX += m_fMoveXpos*0.4f;
				m_Text4.fY += m_fMoveYpos*0.4f;
			}
		}
	}

	return NO_EVENT;
}

void CPlayerInfo::LateUpdate()
{
}

void CPlayerInfo::Render(HDC hDC)
{
	CObj::UpdateRect();
	UpdateTextRect();

	if (m_bIsInfoOn)
	{
		Rectangle(hDC, m_tRect.left, m_tRect.top,
			m_tRect.right, m_tRect.bottom);

		HDC hMemDC = nullptr;
		NULL_CHECK(hMemDC = CBmpMgr::GetInstance()->FindImage(L"Player_Stats"));
		BitBlt(hDC, m_tRect.left, m_tRect.top, (int)m_tInfo.fCX, (int)m_tInfo.fCY, hMemDC, 0, 0, SRCCOPY);

		/*Text배경 흰색 지워줌*/
		SetBkMode(hDC, TRANSPARENT);

		TCHAR szBuf[128] = L"";
		/*PlayerInfo 정보*/
		swprintf_s(szBuf, L"%d\n%s\n\n", m_pPlayer->GetInfo().iLevel, m_pPlayer->GetInfo().szName.c_str());
		DrawText(hDC, szBuf, lstrlen(szBuf), &m_TextRect1, DT_LEFT);

		swprintf_s(szBuf, L"%d\n%d\n%d\n%d\n%d\n%d\n%d",
			m_tAbility.iStap, m_tAbility.iHack, m_tAbility.iInt, m_tAbility.iDef,
			m_tAbility.iMr, m_tAbility.iDex, m_tAbility.iAgl);
		DrawText(hDC, szBuf, lstrlen(szBuf), &m_TextRect2, DT_CENTER);

		swprintf_s(szBuf, L"%d / %d\n%d / %d\n%d / %d\n%d\nATT : %d\n%d / %d",
			m_pPlayer->GetInfo().iHp, m_pPlayer->GetInfo().iMaxHp,
			m_pPlayer->GetInfo().iMp, m_pPlayer->GetInfo().iMaxMp,
			m_pPlayer->GetInfo().iSp, m_pPlayer->GetInfo().iMaxSp,
			m_pPlayer->GetInfo().iGold, m_pPlayer->GetInfo().iAtt,
			m_pPlayer->GetInfo().iExp,m_pPlayer->GetInfo().iMaxExp);

		DrawText(hDC, szBuf, lstrlen(szBuf), &m_TextRect3, DT_CENTER);

		/*EXP BAR*/
		NULL_CHECK(hMemDC = CBmpMgr::GetInstance()->FindImage(L"ExpBar"));
		BitBlt(hDC, m_TextRect4.left, m_TextRect4.top, (int)m_Text4.fCX, (int)m_Text4.fCY,
			hMemDC, 0, 0, SRCCOPY);

		int iExp = m_pPlayer->GetInfo().iExp;
		int iMaxExp = m_pPlayer->GetInfo().iMaxExp;
		
		NULL_CHECK(hMemDC = CBmpMgr::GetInstance()->FindImage(L"Exp"));
		BitBlt(hDC, m_TextRect4.left, m_TextRect4.top, (int)(iExp*EXP_LEN / iMaxExp), (int)m_Text4.fCY,
			hMemDC, 0, 0, SRCCOPY);

		/*Rectangle(hDC, m_TextRect3.left, m_TextRect3.top,
			m_TextRect3.right, m_TextRect3.bottom);*/
	}

}

void CPlayerInfo::Release()
{
}

void CPlayerInfo::UpdateTextRect()
{
	m_TextRect1.top		= (LONG)(m_Text1.fY - m_Text1.fCY*0.5f);
	m_TextRect1.right	= (LONG)(m_Text1.fX + m_Text1.fCX*0.5f);
	m_TextRect1.left	= (LONG)(m_Text1.fX - m_Text1.fCX*0.5f);
	m_TextRect1.bottom  = (LONG)(m_Text1.fY + m_Text1.fCY*0.5f);

	m_TextRect2.left	= (LONG)(m_Text2.fX - m_Text2.fCX*0.5f);
	m_TextRect2.top		= (LONG)(m_Text2.fY - m_Text2.fCY*0.5f);
	m_TextRect2.right	= (LONG)(m_Text2.fX + m_Text2.fCX*0.5f);
	m_TextRect2.bottom	= (LONG)(m_Text2.fY + m_Text2.fCY*0.5f);

	m_TextRect3.left	= (LONG)(m_Text3.fX - m_Text3.fCX*0.5f);
	m_TextRect3.top		= (LONG)(m_Text3.fY - m_Text3.fCY*0.5f);
	m_TextRect3.right	= (LONG)(m_Text3.fX + m_Text3.fCX*0.5f);
	m_TextRect3.bottom	= (LONG)(m_Text3.fY + m_Text3.fCY*0.5f);

	m_TextRect4.left	 = (LONG)(m_Text4.fX - m_Text4.fCX*0.5f);
	m_TextRect4.top		 = (LONG)(m_Text4.fY - m_Text4.fCY*0.5f);
	m_TextRect4.right	 = (LONG)(m_Text4.fX + m_Text4.fCX*0.5f);
	m_TextRect4.bottom	 = (LONG)(m_Text4.fY + m_Text4.fCY*0.5f);
}
