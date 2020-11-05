#include "stdafx.h"
#include "MyButton.h"


CMyButton::CMyButton()
{
}


CMyButton::~CMyButton()
{
	Release();
}

void CMyButton::Initialize()
{
	m_tInfo.fCX = 115.f;
	m_tInfo.fCY = 24.f;

	
}

int CMyButton::Update()
{
	POINT pt = {};
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);

	CObj::UpdateRect();

	

	if (PtInRect(&m_tRect, pt))
	{

		CSoundMgr::GetInstance()->PlaySound(L"over.wav", CSoundMgr::BUTTON_OVER);

		if (KEY_UP(VK_LBUTTON))
		{
			CSoundMgr::GetInstance()->PlaySound(L"Select.wav", CSoundMgr::BUTTON_SELECT);

			if (!lstrcmp(L"Start", m_pImageKey.c_str()))
				CSceneMgr::GetInstance()->SceneChange(CSceneMgr::SC_TOWN);

			else if (!lstrcmp(L"Credit", m_pImageKey.c_str()))
				CSceneMgr::GetInstance()->SceneChange(CSceneMgr::SC_EDIT);

			else if (!lstrcmp(L"Exit", m_pImageKey.c_str()))
				DestroyWindow(g_hWnd);

			return NO_EVENT;
		}
		/*이미지 애니메이션 값.*/
		m_iDrawID = 1;
	}
	else
		m_iDrawID = 0;


	return NO_EVENT;
}

void CMyButton::LateUpdate()
{
}


void CMyButton::Render(HDC hDC)
{
	CObj::UpdateRect();

	HDC hMemDC = nullptr;
	NULL_CHECK(hMemDC = CBmpMgr::GetInstance()->FindImage(m_pImageKey));

	GdiTransparentBlt(hDC, m_tRect.left, m_tRect.top, (int)m_tInfo.fCX, (int)m_tInfo.fCY,
		hMemDC, m_iDrawID*(int)m_tInfo.fCX, 0, (int)m_tInfo.fCX, (int)m_tInfo.fCY, RGB(0, 0, 0));
}

void CMyButton::Release()
{
}
