#include "stdafx.h"
#include "Logo.h"


CLogo::CLogo()
{
}


CLogo::~CLogo()
{
	Release();
}

void CLogo::Initialize()
{
	CSoundMgr::GetInstance()->PlayBGM(L"Vanilla Mood-10-Beyond.mp3");



	CBmpMgr::GetInstance()->InsertBmp(L"MyLogo", L"../Image/Logo/MyLogo.bmp");

	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 67;
	m_tFrame.iScene = 0;
	m_tFrame.dwOldTime = GetTickCount();
	m_tFrame.dwFrameSpd = 90;

	iCnt = 0;
}

void CLogo::Update()
{

}

void CLogo::LateUpdate()
{
	if (KEY_DOWN(VK_RETURN))
	{
		++iCnt;
		++m_tFrame.iFrameStart;

		if (iCnt == 3)
		{
			CSceneMgr::GetInstance()->SceneChange(CSceneMgr::SC_MENU);
			return;
		}
	}
	if(iCnt == 2)
		MoveFrame();
}

void CLogo::Render(HDC hDC)
{
	HDC hMemDC = nullptr;
	NULL_CHECK(hMemDC = CBmpMgr::GetInstance()->FindImage(L"MyLogo"));

	BitBlt(hDC, 0, 0, WINCX, WINCY, hMemDC, m_tFrame.iFrameStart*WINCX, 0, SRCCOPY);
}

void CLogo::Release()
{
	CSoundMgr::GetInstance()->StopSound(CSoundMgr::CHANNEL::BGM);
}


void CLogo::MoveFrame()
{
	if (m_tFrame.dwOldTime + m_tFrame.dwFrameSpd < GetTickCount())
	{
		m_tFrame.iFrameStart++;
		m_tFrame.dwOldTime = GetTickCount();
	}

	if (m_tFrame.iFrameStart > m_tFrame.iFrameEnd)
	{
		//m_tFrame.iFrameStart = 0;
		CSceneMgr::GetInstance()->SceneChange(CSceneMgr::SC_MENU);
		return;
	}
}
