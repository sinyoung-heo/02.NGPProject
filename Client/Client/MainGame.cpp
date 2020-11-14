#include "stdafx.h"
#include "MainGame.h"
#include "Player.h"

#include "../Protocol.h"
using namespace PROTOCOL;

CMainGame::CMainGame()
{
}

CMainGame::~CMainGame()
{
	Release();

	/* 서버 종료 */
	closesocket(g_hSocket);
	WSACleanup();
}

void CMainGame::Initialize()
{
	m_hDC = GetDC(g_hWnd);

	/* 서버 초기화 */
	wcout.imbue(std::locale("korean"));
	Ready_Server();

#pragma region READY_MAINAPP_RESOURCE
	/*사운드*/
	CSoundMgr::GetInstance()->Initialize();

	/*잔상 제거 비트맵*/
	CBmpMgr::GetInstance()->InsertBmp(L"Back", L"../Image/Back.bmp");
	/*후면 버퍼 비트맵*/
	CBmpMgr::GetInstance()->InsertBmp(L"BackBuffer", L"../Image/BackBuffer.bmp");

	/*타일 이미지*/
	CBmpMgr::GetInstance()->InsertBmp(L"tile_0", L"../Image/Tile/tile_0.bmp");
	CBmpMgr::GetInstance()->InsertBmp(L"tile_1", L"../Image/Tile/tile_1.bmp");
	CBmpMgr::GetInstance()->InsertBmp(L"tile_2", L"../Image/Tile/tile_2.bmp");
	CBmpMgr::GetInstance()->InsertBmp(L"tile_3", L"../Image/Tile/tile_3.bmp");
	CBmpMgr::GetInstance()->InsertBmp(L"tile_4", L"../Image/Tile/tile_4.bmp");
	CBmpMgr::GetInstance()->InsertBmp(L"tile_5", L"../Image/Tile/tile_5.bmp");
	CBmpMgr::GetInstance()->InsertBmp(L"tile_6", L"../Image/Tile/tile_6.bmp");

	/*배경 이미지*/
	CBmpMgr::GetInstance()->InsertBmp(L"Town", L"../Image/Map/Town.bmp");
	CBmpMgr::GetInstance()->InsertBmp(L"Store", L"../Image/Map/Store.bmp");
	CBmpMgr::GetInstance()->InsertBmp(L"Field", L"../Image/Map/Field.bmp");
	CBmpMgr::GetInstance()->InsertBmp(L"Dun1", L"../Image/Map/Dun1.bmp");
	CBmpMgr::GetInstance()->InsertBmp(L"Boss", L"../Image/Map/Boss.bmp");

	/*스킬 이미지*/
	CBmpMgr::GetInstance()->InsertBmp(L"Skill_Soul", L"../Image/Skill/Skill_Soul.bmp");
	CBmpMgr::GetInstance()->InsertBmp(L"MultiAttack", L"../Image/Skill/MultiAttack.bmp");
	CBmpMgr::GetInstance()->InsertBmp(L"Skill_Moon", L"../Image/Skill/Skill_Moon.bmp");

	/*Boris 스킬이미지.*/
	CBmpMgr::GetInstance()->InsertBmp(L"ElectricBolt", L"../Image/Skill/ElectricBolt.bmp");
	CBmpMgr::GetInstance()->InsertBmp(L"Skill_Boom", L"../Image/Skill/Skill_Boom.bmp");
	CBmpMgr::GetInstance()->InsertBmp(L"Skill_Quake", L"../Image/Skill/Skill_Quake.bmp");
	CBmpMgr::GetInstance()->InsertBmp(L"WizardBall", L"../Image/Skill/WizardBall.bmp");

	/*피격 Effect*/
	CBmpMgr::GetInstance()->InsertBmp(L"Crash", L"../Image/Effect/Crash.bmp");
	CBmpMgr::GetInstance()->InsertBmp(L"Crash2", L"../Image/Effect/Crash2.bmp");

	/*LefelUp Effect*/
	CBmpMgr::GetInstance()->InsertBmp(L"LvUpEffect", L"../Image/Effect/LvUpEffect.bmp");
	CBmpMgr::GetInstance()->InsertBmp(L"LvUp", L"../Image/Effect/LvUp.bmp");

	/*Monster Hp Bar*/
	CBmpMgr::GetInstance()->InsertBmp(L"Small_HpBar", L"../Image/Ui/Small_HpBar.bmp");
	CBmpMgr::GetInstance()->InsertBmp(L"Small_HpBarEmpty", L"../Image/Ui/Small_HpBarEmpty.bmp");

	/*DmgFont*/
	CBmpMgr::GetInstance()->InsertBmp(L"DmgFont", L"../Image/Effect/DmgFont.bmp");

	/*Potal*/
	CBmpMgr::GetInstance()->InsertBmp(L"Portal", L"../Image/Effect/Portal.bmp");
	CBmpMgr::GetInstance()->InsertBmp(L"Portal2", L"../Image/Effect/Portal2.bmp");
#pragma endregion

	/*마우스 매니저 초기화.*/
	CMouseMgr::GetInstance()->Initialize();
	ShowCursor(false);

	/*첫 화면 LOGO로 초기화.*/
	CSceneMgr::GetInstance()->SceneChange(CSceneMgr::SC_LOGO);
}

void CMainGame::Update()
{
	CSceneMgr::GetInstance()->Update();
}

void CMainGame::LateUpdate()
{
	CSceneMgr::GetInstance()->LateUpdate();
	CKeyMgr::GetInstance()->UpdateKey();

	CSoundMgr::GetInstance()->UpdateSound();
}

void CMainGame::Render()
{
	/*후면 버퍼*/
	HDC hBackBuffer = nullptr;
	NULL_CHECK(hBackBuffer = CBmpMgr::GetInstance()->FindImage(L"BackBuffer"));

	/*잔상 제거*/
	HDC hMemDC = nullptr;
	NULL_CHECK(hMemDC = CBmpMgr::GetInstance()->FindImage(L"Back"));
	/*잔상 제거 비트맵 출력 (후면 버퍼)*/
	BitBlt(hBackBuffer, 0, 0, WINCX, WINCY, hMemDC, 0, 0, SRCCOPY);

	/*후면 버퍼에 1차 출력*/
	CSceneMgr::GetInstance()->Render(hBackBuffer);

	/*후면 버퍼 내용 출력(전면 버퍼)*/
	BitBlt(m_hDC, 0, 0, WINCX, WINCY, hBackBuffer, 0, 0, SRCCOPY);
}

void CMainGame::Release()
{
	ReleaseDC(g_hWnd, m_hDC);

	CBmpMgr::GetInstance()->DestroyInstance();
	CKeyMgr::GetInstance()->DestroyInstance();
	CSceneMgr::GetInstance()->DestroyInstance();
	CMouseMgr::GetInstance()->DestroyInstance();


	CSoundMgr::GetInstance()->DestroyInstance();

	CObjMgr::GetInstance()->DestroyInstance();
}

void CMainGame::Ready_Server()
{
	// Initialize Windows Socket
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	// Create Windows Socket
	g_hSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (g_hSocket == INVALID_SOCKET)
		err_quit("socket()");

	SOCKADDR_IN sockAddr;
	memset(&sockAddr, 0, sizeof(sockAddr));

	sockAddr.sin_family = AF_INET;
	sockAddr.sin_port = htons(PROTOCOL::SERVER_PORT);
	sockAddr.sin_addr.s_addr = inet_addr(PROTOCOL::SERVER_IP);


	if (connect(g_hSocket, (SOCKADDR*)&sockAddr, sizeof(sockAddr)) == SOCKET_ERROR)
	{
		err_quit("connect()");
	}

}

void CMainGame::Connect_Server()
{
}
