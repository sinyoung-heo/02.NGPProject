#include "stdafx.h"
#include "MapButton.h"
#include "MyEdit.h"


CMapButton::CMapButton()
{
}


CMapButton::~CMapButton()
{
	Release();
}

void CMapButton::Initialize()
{
	m_tInfo.fCX = 150.f;
	m_tInfo.fCY = 80.f;
}

int CMapButton::Update()
{
	POINT pt = {};
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);

	CObj::UpdateRect();

	if (PtInRect(&m_tRect,pt))
	{
		if (KEY_UP(VK_LBUTTON))
		{
			if (!lstrcmp(L"Town", m_pMapImageKey))
			{

			}
		}
	}


	return NO_EVENT;
}

void CMapButton::LateUpdate()
{
}

void CMapButton::Render(HDC hDC)
{
	
}

void CMapButton::Release()
{
}
