#include "stdafx.h"
#include "StateButton.h"
#include "PlayerInfo.h"


CStateButton::CStateButton()
{

}

CStateButton::~CStateButton()
{
	Release();
}

void CStateButton::Initialize()
{
	CBmpMgr::GetInstance()->InsertBmp(L"StateBar", L"../Image/Ui/StateBar.bmp");

	m_eLayer = Layer::UI;

	m_tInfo.fCX = 18.f;
	m_tInfo.fCY = 51.f;

	m_iDrawKey = 0;
	m_bIsUiOn = false;
	m_sound = false;

	m_pPlayerInfo = CObjMgr::GetInstance()->GetUiBack();
}

void CStateButton::LateInit()
{

}

int CStateButton::Update()
{
	CObj::LateInit();
	if (m_bIsDead)
		return DEAD_OBJ;


	POINT pt = {};
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);

	if (PtInRect(&m_tRect, pt))
	{
		if (!m_sound)
			CSoundMgr::GetInstance()->PlaySound(L"barover.wav", CSoundMgr::BUTTON_OVER);


		if (KEY_DOWN(VK_LBUTTON))
		{
			CSoundMgr::GetInstance()->PlaySound(L"wclose.wav", CSoundMgr::BUTTON_SELECT);


			m_iDrawKey += 1;
			m_bIsUiOn = !m_bIsUiOn;
			dynamic_cast<CPlayerInfo*>(m_pPlayerInfo)->SetbIsInfoOn(m_bIsUiOn);
		}

		m_sound = true;
	}
	else
		m_sound = false;

	return NO_EVENT;
}

void CStateButton::LateUpdate()
{
	m_iDrawKey = m_iDrawKey % 2;
}

void CStateButton::Render(HDC hDC)
{
	CObj::UpdateRect();

	HDC hMemDC = nullptr;
	NULL_CHECK(hMemDC = CBmpMgr::GetInstance()->FindImage(L"StateBar"));

	//Rectangle

	GdiTransparentBlt(hDC, m_tRect.left, m_tRect.top, (int)m_tInfo.fCX, (int)m_tInfo.fCY,
		hMemDC, m_iDrawKey * (int)m_tInfo.fCX, 0, (int)m_tInfo.fCX, (int)m_tInfo.fCY, RGB(255, 0, 255));
}

void CStateButton::Release()
{

}
