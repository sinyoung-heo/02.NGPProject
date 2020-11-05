#include "stdafx.h"
#include "MyEdit.h"
#include "MapButton.h"


CMyEdit::CMyEdit()
	:m_iDrawID(0)
{
}


CMyEdit::~CMyEdit()
{
	Release();
}

void CMyEdit::Initialize()
{
	/*Edit UI*/
	CBmpMgr::GetInstance()->InsertBmp(L"Edit", L"../Image/Tile/Edit.bmp");
	/*Tile Img*/
	CBmpMgr::GetInstance()->InsertBmp(L"tile_0", L"../Image/Tile/tile_0.bmp");
	CBmpMgr::GetInstance()->InsertBmp(L"tile_1", L"../Image/Tile/tile_1.bmp");
	CBmpMgr::GetInstance()->InsertBmp(L"tile_2", L"../Image/Tile/tile_2.bmp");
	CBmpMgr::GetInstance()->InsertBmp(L"tile_3", L"../Image/Tile/tile_3.bmp");
	CBmpMgr::GetInstance()->InsertBmp(L"tile_4", L"../Image/Tile/tile_4.bmp");
	CBmpMgr::GetInstance()->InsertBmp(L"tile_5", L"../Image/Tile/tile_5.bmp");
	CBmpMgr::GetInstance()->InsertBmp(L"tile_6", L"../Image/Tile/tile_6.bmp");
	/*MpaImage*/
	CBmpMgr::GetInstance()->InsertBmp(L"Town", L"../Image/Map/Town.bmp");
	CBmpMgr::GetInstance()->InsertBmp(L"Store", L"../Image/Map/Store.bmp");
	CBmpMgr::GetInstance()->InsertBmp(L"Field", L"../Image/Map/Field.bmp");
	CBmpMgr::GetInstance()->InsertBmp(L"Dun1", L"../Image/Map/Dun1.bmp");
	CBmpMgr::GetInstance()->InsertBmp(L"Boss", L"../Image/Map/Boss.bmp");

	/*mapImageKey Town으로 초기화.*/
	m_pMapImageKey = L"Town";


	/*Tile Ui X 크기만큼 스크롤 설정*/
	CScrollMgr::SetScrollX(150.f);


	CTileMgr::GetInstance()->Initialize();
	CMouseMgr::GetInstance()->Initialize();
}

void CMyEdit::Update()
{
	if (KEY_PRESSING(VK_LBUTTON))
		IsPicking();

	if (KEY_PRESSING(VK_LEFT))
		CScrollMgr::SetScrollX(5.f);
	if (KEY_PRESSING(VK_RIGHT))
		CScrollMgr::SetScrollX(-5.f);
	if (KEY_PRESSING(VK_UP))
		CScrollMgr::SetScrollY(5.f);
	if (KEY_PRESSING(VK_DOWN))
		CScrollMgr::SetScrollY(-5.f);

	if (KEY_DOWN('1'))
		m_iDrawID = 0;
	if (KEY_DOWN('2'))
		m_iDrawID = 1;
	if (KEY_DOWN('3'))
		m_iDrawID = 2;
	if (KEY_DOWN('4'))
		m_iDrawID = 3;
	if (KEY_DOWN('5'))
		m_iDrawID = 4;
	if (KEY_DOWN('6'))
		m_iDrawID = 5;
	if (KEY_DOWN('7'))
		m_iDrawID = 6;

	/*맵 이미지 변경*/
	if (KEY_DOWN(VK_F1))
		m_pMapImageKey = L"Town";
	if (KEY_DOWN(VK_F2))
		m_pMapImageKey = L"Store";
	if (KEY_DOWN(VK_F3))
		m_pMapImageKey = L"Field";
	if (KEY_DOWN(VK_F4))
		m_pMapImageKey = L"Dun1";
	if (KEY_DOWN(VK_F5))
		m_pMapImageKey = L"Boss";

	/*저장 & 불러오기*/
	if (KEY_DOWN('S'))
		CTileMgr::GetInstance()->SaveData(m_pMapImageKey);
	if (KEY_DOWN('L'))
		CTileMgr::GetInstance()->LoadData(m_pMapImageKey);

	/*씬 전환*/
	if (KEY_DOWN('E'))
		CSceneMgr::GetInstance()->SceneChange(CSceneMgr::SC_MENU);


	CTileMgr::GetInstance()->Update();

	CMouseMgr::GetInstance()->Update();
}

void CMyEdit::LateUpdate()
{
	CTileMgr::GetInstance()->LateUpdate();

	CMouseMgr::GetInstance()->LateUpdate();
}

void CMyEdit::Render(HDC hDC)
{
	HDC hMemDC = nullptr;
	
	NULL_CHECK(hMemDC = CBmpMgr::GetInstance()->FindImage(m_pMapImageKey));
	
	int MapCX = 0;
	int MapCY = 0;
	/*map size마다 크기 조정해준다.*/
	if (!lstrcmp(L"Town", m_pMapImageKey.c_str()))
	{
		MapCX = 1600;
		MapCY = 1200;
	}
	else if (!lstrcmp(L"Store", m_pMapImageKey.c_str()))
	{
		MapCX = 1277;
		MapCY = 991;
	}
	else if (!lstrcmp(L"Field", m_pMapImageKey.c_str()))
	{
		MapCX = 3180;
		MapCY = 1687;
	}
	else if (!lstrcmp(L"Dun1", m_pMapImageKey.c_str()))
	{
		MapCX = 2500;
		MapCY = 1304;
	}
	else if (!lstrcmp(L"Boss", m_pMapImageKey.c_str()))
	{
		MapCX = 3900;
		MapCY = 2040;
	}
	/*맵 출력.*/
	float fScrollX = CScrollMgr::GetScrollX();
	float fScrollY = CScrollMgr::GetScrollY();
	BitBlt(hDC, (int)fScrollX, (int)fScrollY, MapCX, MapCY, hMemDC, 0, 0, SRCCOPY);



	NULL_CHECK(hMemDC = CBmpMgr::GetInstance()->FindImage(L"Edit"));
	/*타일 출력*/
	CTileMgr::GetInstance()->Render(hDC);
	/*Edit UI 출력.*/
	BitBlt(hDC, 0, 0, 150, 600, hMemDC, 0, 0, SRCCOPY);

	/*픽 할 타일 출력*/
	TCHAR pImagekey[8];
	swprintf_s(pImagekey, L"tile_%d", m_iDrawID);

	NULL_CHECK(hMemDC = CBmpMgr::GetInstance()->FindImage(pImagekey));
	BitBlt(hDC, 0, 0, 30, 30, hMemDC, 0, 0, SRCCOPY);
	/*마우스 출력*/
	CMouseMgr::GetInstance()->Render(hDC);
}

void CMyEdit::Release()
{
	CTileMgr::GetInstance()->DestroyInstance();
	CMouseMgr::GetInstance()->DestroyInstance();
}


void CMyEdit::IsPicking()
{
	POINT pt = {};
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);


	float fScrollX = CScrollMgr::GetScrollX();
	float fScrollY = CScrollMgr::GetScrollY();
	

	int x = int(pt.x - fScrollX) / TILECX;
	int y = int(pt.y - fScrollY) / TILECY;

	

	int iIndex = x + (TILEX * y);


	CTileMgr::GetInstance()->PickTile(iIndex, m_iDrawID);
}
