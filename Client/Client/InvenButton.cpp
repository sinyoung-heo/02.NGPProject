#include "stdafx.h"
#include "InvenButton.h"
#include "Inventory.h"

CInvenButton::CInvenButton()
{

}

CInvenButton::~CInvenButton()
{
	Release();
}

void CInvenButton::Initialize()
{
	CBmpMgr::GetInstance()->InsertBmp(L"invenbar", L"../Image/Ui/invenbar.bmp");

	m_eLayer = Layer::UI;

	m_tInfo.fCX = 18.f;
	m_tInfo.fCY = 51.f;

	m_iDrawKey = 0;
	m_bIsUiOn = false;
	m_sound = false;

	m_pPlayerInven = CObjMgr::GetInstance()->GetInventory();
}

void CInvenButton::LateInit()
{

}

int CInvenButton::Update()
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
			static_cast<CInventory*>(m_pPlayerInven)->SetbIsInvenOn(m_bIsUiOn);
		}

		m_sound = true;
	}
	else
		m_sound = false;

	return NO_EVENT;
}

void CInvenButton::LateUpdate()
{
	m_iDrawKey = m_iDrawKey % 2;
}

void CInvenButton::Render(HDC hDC)
{
	CObj::UpdateRect();

	HDC hMemDC = nullptr;
	NULL_CHECK(hMemDC = CBmpMgr::GetInstance()->FindImage(L"invenbar"));

	GdiTransparentBlt(hDC, m_tRect.left, m_tRect.top, (int)m_tInfo.fCX, (int)m_tInfo.fCY,
		hMemDC, m_iDrawKey*(int)m_tInfo.fCX, 0, (int)m_tInfo.fCX, (int)m_tInfo.fCY, RGB(255, 0, 255));
}

void CInvenButton::Release()
{

}
