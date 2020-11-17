#include "stdafx.h"
#include "MainGame.h"
#include "Player.h"



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
	Connect_Server();

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
	/* Non-Blocking Socket으로 전환 */
	unsigned long ul = 1;
	ioctlsocket(g_hSocket, FIONBIO, (unsigned long*)&ul);

	if (g_hSocket == INVALID_SOCKET)
	{
		wchar_t temp[30] = L"Client Socket Create Failed";
		LPCWSTR tempMsg = temp;
		LPVOID	lpMsgBuf;
		FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, WSAGetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&lpMsgBuf, 0, NULL);
		MessageBox(NULL, (LPCTSTR)lpMsgBuf, tempMsg, MB_ICONERROR);
		LocalFree(lpMsgBuf);
		exit(1);
	}
}

int CMainGame::Connect_Server()
{
	SOCKADDR_IN sockAddr;
	memset(&sockAddr, 0, sizeof(sockAddr));

	sockAddr.sin_family = AF_INET;
	sockAddr.sin_port = htons(PROTOCOL_TEST::SERVER_PORT);
	sockAddr.sin_addr.s_addr = inet_addr(PROTOCOL_TEST::SERVER_IP);


	if (connect(g_hSocket, (SOCKADDR*)&sockAddr, sizeof(sockAddr)) == SOCKET_ERROR)
	{
		int state = WSAGetLastError();
		if (state != WSAEWOULDBLOCK)
		{
			WCHAR* h_mess;
			FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
				NULL, WSAGetLastError(),
				MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
				(LPWSTR)&h_mess, 0, NULL);
			cout << "WSAConnect";
			wcout << L"에러 -> " << h_mess << endl;
			while (true);
			LocalFree(h_mess);

			closesocket(g_hSocket);
			return -1;
		}
	}

	cout << "서버에 접속을 요청하였습니다. 잠시만 기다려주세요." << endl;

	/* login_packet 서버로 보내기 */
	cs_packet_login l_packet;
	l_packet.size = sizeof(l_packet);
	l_packet.type = CS_PACKET_LOGIN;
	int t_id = GetCurrentProcessId();
	sprintf_s(l_packet.name, "P%03d", t_id % 1000);
	strcpy_s(g_szServerName, l_packet.name);

	char* p = reinterpret_cast<char*>(&l_packet);

	int iSendBytes = send(g_hSocket, p, p[0], 0);

	if (iSendBytes == SOCKET_ERROR)
	{
		wchar_t temp[30] = L"Client Socket Send Failed";
		LPCWSTR tempMsg = temp;
		LPVOID	lpMsgBuf;
		FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, WSAGetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&lpMsgBuf, 0, NULL);
		MessageBox(NULL, (LPCTSTR)lpMsgBuf, tempMsg, MB_ICONERROR);
		LocalFree(lpMsgBuf);
		closesocket(g_hSocket);
		exit(1);
	}
	cout << "Login packet 전송완료" << endl;

	return NO_EVENT;
}
