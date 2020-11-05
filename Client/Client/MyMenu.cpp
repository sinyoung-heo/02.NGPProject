#include "stdafx.h"
#include "MyMenu.h"
#include "MyButton.h"


CMyMenu::CMyMenu()
{
}

CMyMenu::~CMyMenu()
{
	Release();
}

void CMyMenu::Initialize()
{
	CSoundMgr::GetInstance()->PlayBGM(L"BGM_Menu.mp3");


	CBmpMgr::GetInstance()->InsertBmp(L"Menu", L"../Image/Menu/Menu.bmp");
	CBmpMgr::GetInstance()->InsertBmp(L"Start", L"../Image/Menu/Start.bmp");
	CBmpMgr::GetInstance()->InsertBmp(L"Credit", L"../Image/Menu/Credit.bmp");
	CBmpMgr::GetInstance()->InsertBmp(L"Exit", L"../Image/Menu/Exit.bmp");

	CObj* pStart = CAbstractFactory<CMyButton>::CreateObj(402.f, 405.f);
	CObj* pCredit = CAbstractFactory<CMyButton>::CreateObj(400.f, 451);
	CObj* pExit = CAbstractFactory<CMyButton>::CreateObj(400.f, 475.f);

	static_cast<CMyButton*>(pStart)->SetImageKey(L"Start");
	static_cast<CMyButton*>(pCredit)->SetImageKey(L"Credit");
	static_cast<CMyButton*>(pExit)->SetImageKey(L"Exit");

	CObjMgr::GetInstance()->AddObject(pStart, ObjID::BUTTON);
	CObjMgr::GetInstance()->AddObject(pCredit, ObjID::BUTTON);
	CObjMgr::GetInstance()->AddObject(pExit, ObjID::BUTTON);

	CMouseMgr::GetInstance()->Initialize();
}

void CMyMenu::Update()
{
	CMouseMgr::GetInstance()->Update();
	CObjMgr::GetInstance()->Update();
}

void CMyMenu::LateUpdate()
{
	CMouseMgr::GetInstance()->LateUpdate();
	CObjMgr::GetInstance()->LateUpdate();
}

void CMyMenu::Render(HDC hDC)
{
	HDC hMemDC = nullptr;
	NULL_CHECK(hMemDC = CBmpMgr::GetInstance()->FindImage(L"Menu"));
	
	BitBlt(hDC, 0, 0, WINCX, WINCY, hMemDC, 0, 0, SRCCOPY);

	CObjMgr::GetInstance()->Render(hDC);
	CMouseMgr::GetInstance()->Render(hDC);
}

void CMyMenu::Release()
{
	CObjMgr::GetInstance()->DeleteObj(ObjID::BUTTON);
	CSoundMgr::GetInstance()->StopSound(CSoundMgr::CHANNEL::BGM);
}
